#include "flowview.h"
#include "flowscene.h"

#include <QtWidgets>

FlowView::FlowView(QWidget *parent)
    : QGraphicsView(parent)
{
    setBackgroundBrush(Qt::gray);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void FlowView::zoomIn()
{
    double const step   = 1.2;
    double const factor = std::pow(step, 1.0);

    QTransform t = transform();
    if (t.m11() > 2.0)
        return;

    scale(factor, factor);
}

void FlowView::zoomOut()
{
    double const step   = 1.2;
    double const factor = std::pow(step, -1.0);

    scale(factor, factor);
}

void FlowView::zoomAll()
{

}

void FlowView::zoomToRect(const QRectF &rect)
{

}

FlowView::FlowView(FlowScene *scene, QWidget *parent)
    : FlowView(parent)
{
    setScene(scene);
}

void FlowView::contextMenuEvent(QContextMenuEvent *event)
{
    if (itemAt(event->pos())) {
        QGraphicsView::contextMenuEvent(event);
        return;
    }

    QMenu modelMenu;

    QString skipText = QStringLiteral("skip me");

    // 筛选
    auto *searchLe = new QLineEdit(&modelMenu);
    searchLe->setPlaceholderText(QStringLiteral("Filter"));
    searchLe->setClearButtonEnabled(true);
    auto *searchAction = new QWidgetAction(&modelMenu);
    searchAction->setDefaultWidget(searchLe);

    modelMenu.addAction(searchAction);

    // 将结果树视图添加到上下文菜单
    auto *treeView = new QTreeWidget(&modelMenu);
    treeView->header()->close();
    auto *treeViewAction = new QWidgetAction(&modelMenu);
    treeViewAction->setDefaultWidget(treeView);



    modelMenu.addAction(treeViewAction);


    searchLe->setFocus();
    modelMenu.exec(event->globalPos());
}

void FlowView::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());
    if (d > 0.0)
        zoomIn();
    else
        zoomOut();
}

void FlowView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::RubberBandDrag);
        break;

    default:
        break;
    }

    QGraphicsView::keyPressEvent(event);
}

void FlowView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::ScrollHandDrag);
        break;

    default:
        break;
    }
    QGraphicsView::keyReleaseEvent(event);
}
