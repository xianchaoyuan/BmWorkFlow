#include "flowview.h"
#include "flowscene.h"
#include "node.h"
#include "nodegraphicsobject.h"

#include <QtWidgets>

FlowView::FlowView(FlowScene *scene, QWidget *parent)
    : FlowView(parent)
{
    m_scene_ = scene;
    setScene(scene);
}

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

    QMap<QString, QTreeWidgetItem *> topLevelItems;
    for (auto const &cat : m_scene_->registry().categories()) {
        auto *catItem = new QTreeWidgetItem(treeView);
        catItem->setText(0, cat);
        catItem->setData(0, Qt::UserRole, skipText);
        topLevelItems[cat] = catItem;
    }

    for (auto const &assoc : m_scene_->registry().registeredModelsCategoryAssociation()) {
        auto *parent = topLevelItems[assoc.second];
        auto *modelItem = new QTreeWidgetItem(parent);
        modelItem->setText(0, assoc.first);
        modelItem->setData(0, Qt::UserRole, assoc.first);
    }
    treeView->expandAll();

    connect(treeView, &QTreeWidget::itemClicked, [&](QTreeWidgetItem *item, int) {
        QString modelName = item->data(0, Qt::UserRole).toString();
        if (modelName == skipText) {
            return;
        }

        auto type = m_scene_->registry().create(modelName);
        if (type) {
            auto &node = m_scene_->createNode(std::move(type));
            QPoint pos = event->pos();
            QPointF posView = mapToScene(pos);
            node.nodeGraphicsObject().setPos(posView);
        } else {
            qDebug() << "Model not found";
        }

        modelMenu.close();
    });

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

void FlowView::mousePressEvent(QMouseEvent *event)
{

}

void FlowView::mouseMoveEvent(QMouseEvent *event)
{

}
