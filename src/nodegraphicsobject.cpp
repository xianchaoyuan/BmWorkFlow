#include "nodegraphicsobject.h"
#include "flowscene.h"
#include "node.h"
#include "nodepainter.h"

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

QRectF NodeGraphicsObject::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
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
