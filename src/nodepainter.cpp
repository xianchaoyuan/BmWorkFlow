#include "nodepainter.h"
#include "nodegraphicsobject.h"
#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "node.h"
#include "flowscene.h"

void NodePainter::paint(QPainter *painter, Node &node, const FlowScene &scene)
{
    const NodeGeometry &geom = node.nodeGeometry();
    const NodeState &state = node.nodeState();
    const NodeGraphicsObject &graphicsObject = node.nodeGraphicsObject();

    geom.recalculateSize(painter->font());
    NodeDataModel const *model = node.nodeDataModel();

    drawNodeRect(painter, geom, model, graphicsObject);
    drawNodeCaption(painter, geom, model);

    drawConnectionPoints(painter, geom, state, model, scene);
    drawFilledConnectionPoints(painter, geom, state, model);

    drawEntryLabels(painter, geom, model);

    drawResizeRect(painter, geom, model);
}

void NodePainter::drawNodeRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const NodeGraphicsObject &graphicsObject)
{
    auto color = graphicsObject.isSelected()
            ? Qt::darkYellow
            : Qt::gray;

    if (geom.hovered()) {
        QPen p(color, 4);
        painter->setPen(p);
    } else {
        QPen p(color, 2);
        painter->setPen(p);
    }

    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, geom.height()));
    gradient.setColorAt(0.0, Qt::cyan);
    gradient.setColorAt(1.0, Qt::darkCyan);
    painter->setBrush(gradient);

    float diam = 4;
    QRectF boundary( 0, 0, geom.width(), geom.height());

    const double radius = 3.0;
    painter->drawRoundedRect(boundary, radius, radius);
}

void NodePainter::drawNodeCaption(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
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
    const QFontMetrics &metrics = painter->fontMetrics();

    for (auto type : { PortType::In, PortType::Out }) {

    }
}

void NodePainter::drawConnectionPoints(QPainter *painter, const NodeGeometry &geom, const NodeState &state, const NodeDataModel *model, const FlowScene &scene)
{
    float diameter = 8.0;
    auto  reducedDiameter = diameter * 0.6;

    for(auto portType : { PortType::Out, PortType::In }) {
        size_t n = model->nPorts(portType);

        for (unsigned int i = 0; i < n; ++i) {
            QPointF p = geom.portScenePosition(i, portType);
            painter->setBrush(Qt::green);
            painter->drawEllipse(p,
                                 reducedDiameter,
                                 reducedDiameter);
        }
    }
}

void NodePainter::drawFilledConnectionPoints(QPainter *painter, const NodeGeometry &geom, const NodeState &state, const NodeDataModel *model)
{

}

void NodePainter::drawResizeRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{
    if (model->resizable()) {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(geom.resizeRect());
    }
}
