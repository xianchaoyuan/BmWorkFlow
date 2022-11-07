#include "connectionpainter.h"

#include <QIcon>

#include "connectiongeometry.h"
#include "connectiongraphicsobject.h"
#include "connection.h"
#include "nodedata.h"

static QPainterPath cubicPath(ConnectionGeometry const &geom)
{
    const QPointF &source = geom.source();
    const QPointF &sink   = geom.sink();

    auto c1c2 = geom.pointsC1C2();

    // cubic spline
    QPainterPath cubic(source);
    cubic.cubicTo(c1c2.first, c1c2.second, sink);

    return cubic;
}

QPainterPath ConnectionPainter::getPainterStroke(ConnectionGeometry const &geom)
{
    auto cubic = cubicPath(geom);

    const QPointF &source = geom.source();
    QPainterPath result(source);

    unsigned segments = 20;

    for (auto i = 0ul; i < segments; ++i) {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker; stroker.setWidth(10.0);

    return stroker.createStroke(result);
}

#ifdef NODE_DEBUG_DRAWING
static void debugDrawing(QPainter *painter, Connection const &connection)
{
    Q_UNUSED(painter);
    Q_UNUSED(connection);
    ConnectionGeometry const &geom =
            connection.connectionGeometry();

    {
        const QPointF &source = geom.source();
        const QPointF &sink   = geom.sink();

        auto points = geom.pointsC1C2();

        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);

        painter->drawLine(QLineF(source, points.first));
        painter->drawLine(QLineF(points.first, points.second));
        painter->drawLine(QLineF(points.second, sink));
        painter->drawEllipse(points.first, 3, 3);
        painter->drawEllipse(points.second, 3, 3);

        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubicPath(geom));
    }

    {
        painter->setPen(Qt::yellow);

        painter->drawRect(geom.boundingRect());
    }
}
#endif

static void drawSketchLine(QPainter *painter, Connection const &connection)
{
        QPen p;
        p.setWidth(3.0);
        p.setColor(Qt::red);
        p.setStyle(Qt::DashLine);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        const ConnectionGeometry &geom = connection.connectionGeometry();

        auto cubic = cubicPath(geom);
        // cubic spline
        painter->drawPath(cubic);
}

static void drawHoveredOrSelected(QPainter *painter, const Connection &connection)
{
    const ConnectionGeometry &geom = connection.connectionGeometry();
    const bool hovered = geom.hovered();

    const auto &graphicsObject = connection.connectionGraphicsObject();

    const bool selected = graphicsObject.isSelected();

    // drawn as a fat background
    if (hovered || selected) {
        QPen p;
        const double lineWidth = 3.0;

        p.setWidth(2 * lineWidth);
        p.setColor(selected ?
                       Qt::black :
                       Qt::yellow);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        auto cubic = cubicPath(geom);
        painter->drawPath(cubic);
    }
}

static void drawNormalLine(QPainter *painter, Connection const &connection)
{
    // colors
    QColor normalColorOut  = Qt::red;
    QColor normalColorIn   = Qt::cyan;
    QColor selectedColor = Qt::blue;

    bool gradientColor = false;

    // geometry
    const ConnectionGeometry &geom = connection.connectionGeometry();

    const double lineWidth = 3.0;

    // draw normal line
    QPen p;
    p.setWidth(lineWidth);

    const auto &graphicsObject = connection.connectionGraphicsObject();
    const bool selected = graphicsObject.isSelected();

    auto cubic = cubicPath(geom);
    if (gradientColor) {
        painter->setBrush(Qt::NoBrush);

        QColor cOut = normalColorOut;
        if (selected)
            cOut = cOut.darker(200);
        p.setColor(cOut);
        painter->setPen(p);

        const unsigned int segments = 60;

        for (unsigned int i = 0ul; i < segments; ++i) {
            double ratioPrev = double(i) / segments;
            double ratio = double(i + 1) / segments;

            if (i == segments / 2) {
                QColor cIn = normalColorIn;
                if (selected)
                    cIn = cIn.darker(200);

                p.setColor(cIn);
                painter->setPen(p);
            }
            painter->drawLine(cubic.pointAtPercent(ratioPrev),
                              cubic.pointAtPercent(ratio));
        }

        {
            QIcon icon(":convert.png");

            QPixmap pixmap = icon.pixmap(QSize(22, 22));
            painter->drawPixmap(cubic.pointAtPercent(0.50) - QPoint(pixmap.width()/2,
                                                                    pixmap.height()/2),
                                pixmap);

        }
    } else {
        p.setColor(normalColorOut);

        if (selected) {
            p.setColor(selectedColor);
        }

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubic);
    }
}

void ConnectionPainter::paint(QPainter *painter, Connection const &connection)
{
//    drawHoveredOrSelected(painter, connection);
//    drawSketchLine(painter, connection);
    drawNormalLine(painter, connection);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, connection);
#endif

    // draw end points
    ConnectionGeometry const &geom = connection.connectionGeometry();

    const QPointF &source = geom.source();
    const QPointF &sink   = geom.sink();

    const double pointDiameter = 8.0;

    painter->setPen(Qt::red);
    painter->setBrush(Qt::green);
    const double pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(source, pointRadius, pointRadius);
    painter->drawEllipse(sink, pointRadius, pointRadius);
}
