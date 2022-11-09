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
    const NodeDataModel *model = node.nodeDataModel();

#ifdef NODE_DEBUG_DRAWING
    drawTestText(painter, geom, model);
#endif

    drawNodeRect(painter, geom, model, graphicsObject);
    drawNodeCaption(painter, geom, model);

    drawConnectionPoints(painter, geom, state, model, scene);
    drawFilledConnectionPoints(painter, geom, state, model);

    drawEntryLabels(painter, geom, state, model);
//    drawBackgroundPixmap(painter, geom, model, graphicsObject);
}

void NodePainter::drawTestText(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{
    painter->drawText(0, 0, model->getTestText());
}

void NodePainter::drawBackgroundPixmap(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const NodeGraphicsObject &graphicsObject)
{
    QPixmap bkgPixmap = model->backgroundPixmap();
    painter->drawPixmap(QPointF(0, 0), bkgPixmap);
}

void NodePainter::drawNodeRect(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model, const NodeGraphicsObject &graphicsObject)
{
    auto color = graphicsObject.isSelected()
            ? Qt::darkYellow
            : Qt::gray;

    if (geom.hovered()) {
        QPen p(color, 3);
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

    const double radius = 8.0;
    QRectF boundary( 0, 0, geom.width(), geom.height());
    painter->drawRoundedRect(boundary, radius, radius);
}

void NodePainter::drawNodeCaption(QPainter *painter, const NodeGeometry &geom, const NodeDataModel *model)
{
    if (!model->captionVisible())
        return;

    QFont f = painter->font();
    f.setBold(true);

    QFontMetrics metrics(f);
    const QString &name = model->caption();
    auto rect = metrics.boundingRect(name);
    QPointF position((geom.width() - rect.width()) / 2.0,
                     (geom.spacing() + geom.entryHeight()) / 3.0);

    painter->setFont(f);
    painter->setPen(Qt::black);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void NodePainter::drawEntryLabels(QPainter *painter, const NodeGeometry &geom, const NodeState &state, const NodeDataModel *model)
{
    const QFontMetrics &metrics = painter->fontMetrics();

    for (auto &portType : { PortType::In, PortType::Out }) {
        auto &entries = state.getEntries(portType);

        size_t n = entries.size();
        for (size_t i = 0; i < n; ++i) {
            QPointF p = geom.portScenePosition(static_cast<PortIndex>(i), portType);
            if (entries[i].empty())
                painter->setPen(Qt::green);
            else
                painter->setPen(Qt::black);

            QString s;
            if (model->portCaptionVisible(portType, static_cast<PortIndex>(i))) {
                s = model->portCaption(portType, static_cast<PortIndex>(i));
            } else {
                s = model->dataType(portType, static_cast<int>(i)).name;
            }

            auto rect = metrics.boundingRect(s);
            p.setY(p.y() + rect.height() / 4.0);

            switch (portType) {
            case PortType::In:
                p.setX(8.0);
                break;
            case PortType::Out:
                p.setX(geom.width() - 8.0 - rect.width());
                break;

            default:
                break;
            }

            painter->drawText(p, s);
        }
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
