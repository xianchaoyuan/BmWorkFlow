#include "connectiongraphicsobject.h"
#include "flowscene.h"
#include "connection.h"
#include "connectiongeometry.h"
#include "connectionpainter.h"
#include "nodeconnectioninteraction.h"
#include "nodegraphicsobject.h"

#include <QGraphicsView>
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
    for (PortType portType : { PortType::In, PortType::Out } ) {
        if (auto node = m_connection_.getNode(portType)) {
            const auto &nodeGraphics = node->nodeGraphicsObject();
            const auto &nodeGeom = node->nodeGeometry();
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

    // BmTODO 添加port靠近扩大

    QPointF offset = event->pos() - event->lastPos();
    auto requiredPort = m_connection_.requiredPort();
    if (requiredPort != PortType::None) {
        m_connection_.connectionGeometry().moveEndPoint(requiredPort, offset);
    }

    update();
    event->accept();
}

void ConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ungrabMouse();
    event->accept();

    // BmTODO 判断释放时是否在port上
    auto node = locateNodeAt(event->scenePos(),
                             m_scene_,
                             m_scene_.views()[0]->transform());

    NodeConnectionInteraction interaction(*node, m_connection_, m_scene_);
    if (node && interaction.tryConnect()) {
        // BmTODO 恢复端口扩大状态
    }

    // 删除当前连接
    if (m_connection_.connectionState().requiresPort()) {
        m_scene_.deleteConnection(m_connection_);
    }
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
