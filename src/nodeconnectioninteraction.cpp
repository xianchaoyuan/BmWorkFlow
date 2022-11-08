#include "nodeconnectioninteraction.h"
#include "nodegeometry.h"
#include "nodegraphicsobject.h"
#include "flowscene.h"

NodeConnectionInteraction::NodeConnectionInteraction(Node &node,
                                                     Connection &connection,
                                                     FlowScene &scene)
    : m_node_(&node),
      m_connection_(&connection),
      m_scene_(&scene)
{
}

bool NodeConnectionInteraction::canConnection(PortIndex &portIndex, TypeConverter &converter) const
{
    // 类型判断
    PortType requiredType = connectionRequiredPort();
    if (requiredType == PortType::None) {
        return false;
    }

    // 禁止将节点连接到自身
    auto *node = m_connection_->getNode(oppositePort(requiredType));
    if (node == m_node_) {
        return false;
    }

    // 连接点位于节点端口的顶部
    QPointF connectionPoint = connectionEndScenePosition(requiredType);
    portIndex = nodePortIndexUnderScenePoint(requiredType,
                                             connectionPoint);
    if (portIndex == INVALID) {
        return false;
    }

    // 节点端口为空
    if (!nodePortIsEmpty(requiredType, portIndex)) {
        return false;
    }

    // 连接类型等于节点端口类型，或者存在可以在两者之间转换的注册类型转换
    auto connectionDataType = m_connection_->dataType(oppositePort(requiredType));
    auto const &modelTarget = m_node_->nodeDataModel();
    auto candidateNodeDataType = modelTarget->dataType(requiredType, portIndex);
    if (connectionDataType.id != candidateNodeDataType.id) {
        if (requiredType == PortType::In) {
            converter = m_scene_->registry().getTypeConverter(connectionDataType, candidateNodeDataType);
        } else if (requiredType == PortType::Out) {
            converter = m_scene_->registry().getTypeConverter(candidateNodeDataType, connectionDataType);
        }

        return converter != nullptr;
    }

    return true;
}

bool NodeConnectionInteraction::tryConnect() const
{
    PortIndex portIndex = INVALID;
    TypeConverter converter;
    if (!canConnection(portIndex, converter)) {
        return false;
    }

    if (converter) {
        m_connection_->setTypeConverter(converter);
    }

    // 将节点分配到连接中所需的端口
    auto requiredType = connectionRequiredPort();
    m_node_->nodeState().setConnection(requiredType,
                                       portIndex,
                                       *m_connection_);

    // 将连接分配给NodeState中的空端口
    m_connection_->setNodeToPort(*m_node_, requiredType, portIndex);

    // 调整连接几何图形
    m_node_->nodeGraphicsObject().moveConnections();

    // 用于初始化数据传输的Poke模型
    auto outNode = m_connection_->getNode(PortType::Out);
    if (outNode) {
        PortIndex outPortIndex = m_connection_->getPortIndex(PortType::Out);
        outNode->onDataUpdated(outPortIndex);
    }

    return true;
}

bool NodeConnectionInteraction::disconnect(PortType portToDisconnect) const
{
    PortIndex portIndex = m_connection_->getPortIndex(portToDisconnect);

    NodeState &state = m_node_->nodeState();

    // 清除指向节点状态中连接的指针
    state.getEntries(portToDisconnect)[portIndex].erase(m_connection_->id());

    // 将无效数据传播到IN节点
//    m_connection_->

    // 清除连接侧
    m_connection_->clearNode(portToDisconnect);
    m_connection_->setRequiredPort(portToDisconnect);
    m_connection_->connectionGraphicsObject().grabMouse();

    return true;
}

PortType NodeConnectionInteraction::connectionRequiredPort() const
{
    const auto &state = m_connection_->connectionState();
    return state.requiredPort();
}

QPointF NodeConnectionInteraction::connectionEndScenePosition(PortType portType) const
{
    auto &graphicsObject = m_connection_->connectionGraphicsObject();
    auto &geometry = m_connection_->connectionGeometry();

    QPointF endPoint = geometry.getEndPoint(portType);
    return graphicsObject.mapToScene(endPoint);
}

QPointF NodeConnectionInteraction::nodePortScenePosition(PortType portType, PortIndex portIndex) const
{
    const auto &geom = m_node_->nodeGeometry();
    QPointF pos = geom.portScenePosition(portIndex, portType);
    auto &nodeGraphicsObj = m_node_->nodeGraphicsObject();
    return nodeGraphicsObj.sceneTransform().map(pos);
}

PortIndex NodeConnectionInteraction::nodePortIndexUnderScenePoint(PortType portType, const QPointF &scenePoint) const
{
    const auto &nodeGeom = m_node_->nodeGeometry();
    QTransform sceneTransform = m_node_->nodeGraphicsObject().sceneTransform();
    auto portIndex = nodeGeom.checkHitScenePoint(portType,
                                                 scenePoint,
                                                 sceneTransform);
    return portIndex;
}

bool NodeConnectionInteraction::nodePortIsEmpty(PortType portType, PortIndex portIndex) const
{
    const auto &nodeState = m_node_->nodeState();
    const auto &entries = nodeState.getEntries(portType);
    const auto &connections = entries[portIndex];
    if (connections.empty()) {
        return true;
    }

    // 检查连接是否已连接到相应的输入和输出端口
    auto sourcePortType = oppositePort(portType);
    auto it = std::find_if(connections.begin(), connections.end(),
                           [this, sourcePortType](const auto &connection)
    {
        const auto *const currentConn = connection.second;

        Q_ASSERT(m_connection_->getNode(sourcePortType));
        Q_ASSERT(currentConn->getNode(sourcePortType));
        return m_connection_->getNode(sourcePortType) == currentConn->getNode(sourcePortType) &&
                m_connection_->getPortIndex(sourcePortType) == currentConn->getPortIndex(sourcePortType);

    });

    if (it != connections.end())
        return false;

    // 连接策略
    switch (portType) {
    case PortType::In: {
        const auto policy = m_node_->nodeDataModel()->portInConnectionPolicy(portIndex);
        return policy == NodeDataModel::ConnectionPolicy::Many;
    }
    case PortType::Out: {
        const auto policy = m_node_->nodeDataModel()->portOutConnectionPolicy(portIndex);
        return policy == NodeDataModel::ConnectionPolicy::Many;
    }
    default: return false;
    }
}
