#include "nodegraphicsobject.h"
#include "flowscene.h"
#include "node.h"
#include "nodepainter.h"
#include "nodestate.h"
#include "connection.h"
#include "nodeconnectioninteraction.h"

#include <QMenu>

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
    const NodeState &nodeState = m_node_.nodeState();
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
        moveConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto portToCheck : { PortType::In, PortType::Out }) {
        auto &nodeGeometry = m_node_.nodeGeometry();

        const int portIndex = nodeGeometry.checkHitScenePoint(portToCheck,
                                                              event->scenePos(),
                                                              sceneTransform());
        if (portIndex != INVALID) {
            const NodeState &nodeState = m_node_.nodeState();
            std::unordered_map<QUuid, Connection *> connections = nodeState.connections(portToCheck, portIndex);

            // ??????????????????
            if (!connections.empty() && portToCheck == PortType::In) {
                // ????????????
                auto connection = connections.begin()->second;
                NodeConnectionInteraction interaction(m_node_, *connection, m_scene_);
                interaction.disconnect(portToCheck);
            } else {
                if (portToCheck == PortType::Out) {
                    const auto outPolicy = m_node_.nodeDataModel()->portOutConnectionPolicy(portIndex);
                    // ??????????????????????????????One????????????????????????
                    if (!connections.empty() &&
                            outPolicy == NodeDataModel::ConnectionPolicy::One) {
                        m_scene_.deleteConnection(*connections.begin()->second);
                    }
                }

                // ???????????????
                auto connection = m_scene_.createConnection(portToCheck,
                                                            m_node_,
                                                            portIndex);

                m_node_.nodeState().setConnection(portToCheck,
                                                  portIndex,
                                                  *connection);

                connection->connectionGraphicsObject().grabMouse();
            }
        }
    }
}

void NodeGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // BmTODO ??????Qt??????????????????
    if (!isSelected()) {
        m_scene_.clearSelection();
        setSelected(true);
    }
    QGraphicsObject::mouseMoveEvent(event);

    QRectF rect = scene()->sceneRect();
    rect.united(mapToScene(boundingRect()).boundingRect());
    scene()->setSceneRect(rect);
}

void NodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
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

    // ?????????????????????
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

void NodeGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // BmTODO??????????????????????????????
    emit openPropertyWidget();
//    QGraphicsItem::mouseDoubleClickEvent(event);
}

void NodeGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // BmTODO??????????????????
    QMenu menu;

    auto *deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    menu.addAction(deleteAction);

    menu.exec(event->screenPos());
}

