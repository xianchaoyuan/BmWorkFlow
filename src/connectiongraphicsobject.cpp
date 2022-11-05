#include "connectiongraphicsobject.h"
#include "flowscene.h"
#include "connection.h"
#include "connectiongeometry.h"
#include "connectionpainter.h"
#include "nodegraphicsobject.h"

#include <QGraphicsBlurEffect>
#include <QStyleOptionGraphicsItem>

ConnectionGraphicsObject::ConnectionGraphicsObject(FlowScene &scene, Connection &connection)
    : m_scene_(scene),
      m_connection_(connection)
{
    m_scene_.addItem(this);

    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemIsSelectable);

    setAcceptHoverEvents(true);
    addGraphicsEffect();

    setZValue(-1.0);
}

ConnectionGraphicsObject::~ConnectionGraphicsObject()
{

}

void ConnectionGraphicsObject::move()
{
    for (PortType portType: { PortType::In, PortType::Out } ) {
        if (auto node = m_connection_.getNode(portType)) {
            auto const &nodeGraphics = node->nodeGraphicsObject();
            auto const &nodeGeom = node->nodeGeometry();
            QPointF scenePos = nodeGeom.portScenePosition(m_connection_.getPortIndex(portType),
                                               portType,
                                               nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();
            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            m_connection_.connectionGeometry().setEndPoint(portType, connectionPos);
            m_connection_.connectionGraphicsObject().prepareGeometryChange();
            m_connection_.connectionGraphicsObject().update();
        }
    }
}

QRectF ConnectionGraphicsObject::boundingRect() const
{
    return m_connection_.connectionGeometry().boundingRect();
}

QPainterPath ConnectionGraphicsObject::shape() const
{
#ifdef DEBUG_DRAWING
    QPainterPath path;
    path.addRect(boundingRect());
    return path;

#else
    const auto &geom =
            m_connection_.connectionGeometry();

    return ConnectionPainter::getPainterStroke(geom);

#endif
}

void ConnectionGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);
    ConnectionPainter::paint(painter, m_connection_);
}

void ConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void ConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();

    //-------------------
    QPointF offset = event->pos() - event->lastPos();
//    auto requiredPort = m_connection_.requiredPort();
//    if (requiredPort != PortType::None) {
//        m_connection_.connectionGeometry().moveEndPoint(requiredPort, offset);
//    }
    m_connection_.connectionGeometry().moveEndPoint(PortType::Out, offset);
    //-------------------

    update();
    event->accept();
}

void ConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ungrabMouse();
    event->accept();
}

void ConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

}

void ConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

}

void ConnectionGraphicsObject::addGraphicsEffect()
{
//    auto effect = new QGraphicsBlurEffect;

//    effect->setBlurRadius(5);
//    setGraphicsEffect(effect);
}
