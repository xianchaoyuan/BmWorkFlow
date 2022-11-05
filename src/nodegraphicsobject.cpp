#include "nodegraphicsobject.h"
#include "flowscene.h"
#include "node.h"
#include "nodepainter.h"
#include "nodestate.h"
#include "connection.h"

NodeGraphicsObject::NodeGraphicsObject(FlowScene &scene, Node &node)
    : m_scene_(scene),
      m_node_(node)
{
    m_scene_.addItem(this);

    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    setZValue(0);
}

NodeGraphicsObject::~NodeGraphicsObject()
{
    m_scene_.removeItem(this);
}

void NodeGraphicsObject::moveConnections() const
{
    NodeState const &nodeState = m_node_.nodeState();
    for (PortType portType : { PortType::In, PortType::Out }) {
        const auto &connectionEntries =
                nodeState.getEntries(portType);

        for (const auto &connections : connectionEntries) {
            for (auto &con : connections)
                con.second->connectionGraphicsObject().move();
        }
    }
}

QRectF NodeGraphicsObject::boundingRect() const
{
    return m_node_.nodeGeometry().boundingRect();
}

void NodeGraphicsObject::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    painter->setClipRect(option->exposedRect);

    NodePainter::paint(painter, m_node_, m_scene_);
}

QVariant NodeGraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // BmTODO move connections

    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto portType : { PortType::In, PortType::Out }) {
        auto &nodeGeometry = m_node_.nodeGeometry();

        const int portIndex = nodeGeometry.checkHitScenePoint(portType,
                                                              event->scenePos(),
                                                              sceneTransform());
        if (portIndex != INVALID) {
            const NodeState &nodeState = m_node_.nodeState();
            std::unordered_map<QUuid, Connection *> connections = nodeState.connections(portType, portIndex);

            // start dragging existing connection
            if (!connections.empty() && portType == PortType::In) {
                // BmTODO移除连接
                auto con = connections.begin()->second;

            } else {
                if (portType == PortType::Out) {
                    const auto outPolicy = m_node_.nodeDataModel()->portOutConnectionPolicy(portIndex);
                    if (!connections.empty() &&
                            outPolicy == NodeDataModel::ConnectionPolicy::One) {
                        //                        m_scene_.deleteConnection(*connections.begin()->second);
                    }
                }

                // todo add to FlowScene
                auto connection = m_scene_.createConnection(portType,
                                                            m_node_,
                                                            portIndex);

                m_node_.nodeState().setConnection(portType,
                                                  portIndex,
                                                  *connection);

                connection->connectionGraphicsObject().grabMouse();
            }
        }
    }

    // resizing
    auto pos = event->pos();
    auto &geom = m_node_.nodeGeometry();
    auto &state = m_node_.nodeState();

    if (m_node_.nodeDataModel()->resizable() &&
            geom.resizeRect().contains(pos.toPoint())) {
        state.setResizing(true);
    }
}

void NodeGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
        m_scene_.clearSelection();
        setSelected(true);
    }

    auto &geom  = m_node_.nodeGeometry();
    auto &state = m_node_.nodeState();

    if (state.resizing()) {
        auto diff = event->pos() - event->lastPos();
        // 没有嵌入窗体的node不可改变大小
        if (auto *widget = m_node_.nodeDataModel()->embeddedWidget()) {
            prepareGeometryChange();

            auto size = widget->size();
            size += QSize(diff.x(), diff.y());
            widget->setFixedSize(size);

            update();
        }

        event->accept();
    } else {
        QGraphicsObject::mouseMoveEvent(event);

        // 刷新连接
        if (event->lastPos() != event->pos()) {
            moveConnections();
        }

        event->ignore();
    }

    QRectF rect = scene()->sceneRect();
    rect.united(mapToScene(boundingRect()).boundingRect());
    scene()->setSceneRect(rect);
}

void NodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    auto &state = m_node_.nodeState();
    state.setResizing(false);

    QGraphicsObject::mouseReleaseEvent(event);

    moveConnections();
}

void NodeGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QList<QGraphicsItem *> overlapItems = collidingItems();
    for (auto &item : overlapItems) {
        if (item->zValue() > 0.0) {
            item->setZValue(0.0);
        }
    }

    // 向前移动此节点
    setZValue(1.0);

    m_node_.nodeGeometry().setHovered(true);
    update();

    event->accept();
}

void NodeGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_node_.nodeGeometry().setHovered(false);
    update();

    event->accept();
}

void NodeGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    auto pos   = event->pos();
    auto &geom = m_node_.nodeGeometry();

    if (m_node_.nodeDataModel()->resizable() &&
            geom.resizeRect().contains(QPoint(pos.x(), pos.y()))) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    } else {
        setCursor(QCursor());
    }

    event->accept();
}
