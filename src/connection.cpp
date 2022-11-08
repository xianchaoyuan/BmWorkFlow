#include "connection.h"
#include "node.h"
#include "nodedatamodel.h"
#include "nodegraphicsobject.h"

Connection::Connection(PortType portType, Node &node, PortIndex portIndex)
    : m_uuid_(QUuid::createUuid()),
      m_in_port_index_(INVALID),
      m_out_port_index_(INVALID),
      m_state_()
{
    setNodeToPort(node, portType, portIndex);
    setRequiredPort(oppositePort(portType));
}

Connection::~Connection()
{
    if (m_in_node_) {
        m_in_node_->nodeGraphicsObject().update();
    }

    if (m_out_node_) {
        propagateEmptyData();
        m_out_node_->nodeGraphicsObject().update();
    }
}

const ConnectionState &Connection::connectionState() const
{
    return m_state_;
}

ConnectionState &Connection::connectionState()
{
    return m_state_;
}

ConnectionGeometry &Connection::connectionGeometry()
{
    return m_geometry_;
}

const ConnectionGeometry &Connection::connectionGeometry() const
{
    return m_geometry_;
}

ConnectionGraphicsObject &Connection::connectionGraphicsObject()
{
    return *m_graphics_object_;
}

const ConnectionGraphicsObject &Connection::connectionGraphicsObject() const
{
    return *m_graphics_object_;
}

void Connection::setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject> &&graphics)
{
    m_graphics_object_ = std::move(graphics);

    if (requiredPort() != PortType::None) {

        PortType attachedPort = oppositePort(requiredPort());
        PortIndex attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform =
                node->nodeGraphicsObject().sceneTransform();
        QPointF pos = node->nodeGeometry().portScenePosition(attachedPortIndex,
                                                             attachedPort,
                                                             nodeSceneTransform);

        m_graphics_object_->setPos(pos);
    }

    m_graphics_object_->move();
}

Node *Connection::getNode(PortType portType) const
{
    switch (portType)
    {
    case PortType::In:
        return m_in_node_;
        break;

    case PortType::Out:
        return m_out_node_;
        break;

    default:
        // not possible
        break;
    }
    return nullptr;
}

Node *&Connection::getNode(PortType portType)
{
    switch (portType)
    {
    case PortType::In:
        return m_in_node_;
        break;

    case PortType::Out:
        return m_out_node_;
        break;

    default:
        // not possible
        break;
    }
    Q_UNREACHABLE();
}

void Connection::setNodeToPort(Node &node, PortType portType, PortIndex portIndex)
{
    auto &nodeWeak = getNode(portType);
    nodeWeak = &node;

    if (portType == PortType::Out)
        m_out_port_index_ = portIndex;
    else
        m_in_port_index_ = portIndex;

    m_state_.setNoRequiredPort();
}

void Connection::removeFromNodes() const
{
    if (m_in_node_)
        m_in_node_->nodeState().eraseConnection(PortType::In, m_in_port_index_, id());

    if (m_out_node_)
        m_out_node_->nodeState().eraseConnection(PortType::Out, m_out_port_index_, id());
}

void Connection::clearNode(PortType portType)
{
    // BmTODO 如果连接完整，则通知外部断开连接

    getNode(portType) = nullptr;

    if (portType == PortType::In) {
        m_in_port_index_ = INVALID;
    } else {
        m_out_port_index_ = INVALID;
    }
}

void Connection::setRequiredPort(PortType portType)
{
    m_state_.setRequiredPort(portType);

    switch (portType)
    {
    case PortType::Out:
        m_out_node_  = nullptr;
        m_out_port_index_ = INVALID;
        break;

    case PortType::In:
        m_in_node_  = nullptr;
        m_in_port_index_ = INVALID;
        break;

    default:
        break;
    }
}

PortType Connection::requiredPort() const
{
    return m_state_.requiredPort();
}

PortIndex Connection::getPortIndex(PortType portType) const
{
    PortIndex result = INVALID;
    switch (portType)
    {
    case PortType::In:
        result = m_in_port_index_;
        break;

    case PortType::Out:
        result = m_out_port_index_;

        break;

    default:
        break;
    }

    return result;
}

NodeDataType Connection::dataType(PortType portType) const
{
    if (m_in_node_ && m_out_node_) {
        auto const &model = (portType == PortType::In) ?
                    m_in_node_->nodeDataModel() :
                    m_out_node_->nodeDataModel();
        PortIndex index = (portType == PortType::In) ?
                    m_in_port_index_ :
                    m_out_port_index_;

        return model->dataType(portType, index);
    } else {
        Node *validNode;
        PortIndex index = INVALID;

        if ((validNode = m_in_node_)) {
            index    = m_in_port_index_;
            portType = PortType::In;
        } else if ((validNode = m_out_node_)) {
            index    = m_out_port_index_;
            portType = PortType::Out;
        }

        if (validNode) {
            auto const &model = validNode->nodeDataModel();
            return model->dataType(portType, index);
        }
    }

    Q_UNREACHABLE();
}

void Connection::propagateData(std::shared_ptr<NodeData> nodeData) const
{
    if (m_in_node_) {
        if (m_converter_) {
            nodeData = m_converter_(nodeData);
        }

        m_in_node_->propagateData(nodeData, m_in_port_index_, id());
    }
}

void Connection::propagateEmptyData() const
{
    std::shared_ptr<NodeData> emptyData;
    propagateData(emptyData);
}

void Connection::setTypeConverter(TypeConverter converter)
{
    m_converter_ = std::move(converter);
}


