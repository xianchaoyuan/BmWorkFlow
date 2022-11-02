#include "nodepainter.h"
#include "nodegraphicsobject.h"
#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "node.h"
#include "flowscene.h"

void NodePainter::paint(QPainter *painter, Node &node, const FlowScene &scene)
{
    const NodeGeometry &geom = node.nodeGeometry();
    const NodeGraphicsObject &graphicsObject = node.nodeGraphicsObject();

    geom.recalculateSize(painter->font());

    //--------------------------------------------
    NodeDataModel const *model = node.nodeDataModel();

    drawNodeRect(painter, geom, model, graphicsObject);

    drawConnectionPoints(painter, geom, model, scene);

    drawFilledConnectionPoints(painter, geom, model);

    drawModelName(painter, geom, model);

    drawEntryLabels(painter, geom, model);

    drawResizeRect(painter, geom, model);

    drawValidationRect(painter, geom, model, graphicsObject);
}

void NodePainter::drawNodeRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const NodeGraphicsObject &graphicsObject)
{
    auto color = graphicsObject.isSelected()
            ? Qt::blue
            : Qt::green;

    if (geom.hovered()) {
        QPen p(color, 4);
        painter->setPen(p);
    } else {
        QPen p(color, 2);
        painter->setPen(p);
    }

    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, geom.height()));

    gradient.setColorAt(0.0, Qt::gray);
    gradient.setColorAt(1.0, Qt::darkGray);

    painter->setBrush(gradient);

    float diam = 4;

    QRectF boundary( -diam, -diam, 2.0 * diam + geom.width(), 2.0 * diam + geom.height());

    double const radius = 3.0;

    painter->drawRoundedRect(boundary, radius, radius);
}

void NodePainter::drawModelName(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{
    if (!model->captionVisible())
        return;

    QString const &name = model->caption();

    QFont f = painter->font();
    f.setBold(true);

    QFontMetrics metrics(f);
    auto rect = metrics.boundingRect(name);

    QPointF position((geom.width() - rect.width()) / 2.0,
                     (geom.spacing() + geom.entryHeight()) / 3.0);

    painter->setFont(f);
    painter->setPen(Qt::black);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void NodePainter::drawEntryLabels(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{

}

void NodePainter::drawConnectionPoints(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const FlowScene &scene)
{

}

void NodePainter::drawFilledConnectionPoints(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{

}

void NodePainter::drawResizeRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{

}

void NodePainter::drawValidationRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const NodeGraphicsObject &graphicsObject)
{

}
