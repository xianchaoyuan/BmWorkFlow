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

    // setup actions
    m_act_clear_selection_ = new QAction(QStringLiteral("Clear Selection"), this);
    m_act_clear_selection_->setShortcut(Qt::Key_Escape);
    connect(m_act_clear_selection_, &QAction::triggered, m_scene_, &QGraphicsScene::clearSelection);
    addAction(m_act_clear_selection_);

    m_act_delete_selection_ = new QAction(QStringLiteral("Delete Selection"), this);
    m_act_delete_selection_->setShortcut(Qt::Key_Delete);
    connect(m_act_delete_selection_, &QAction::triggered, this, &FlowView::deleteSelectedNodes);
    addAction(m_act_delete_selection_);
}

FlowView::FlowView(QWidget *parent)
    : QGraphicsView(parent)
{
//    setBackgroundBrush(Qt::black);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

void FlowView::zoomIn()
{
    const double step   = 1.2;
    const double factor = std::pow(step, 1.0);

    QTransform t = transform();
    if (t.m11() > 2.0)
        return;

    scale(factor, factor);
}

void FlowView::zoomOut()
{
    const double step   = 1.2;
    const double factor = std::pow(step, -1.0);

    scale(factor, factor);
}

void FlowView::zoomAll()
{

}

void FlowView::zoomToRect(const QRectF &rect)
{

}

void FlowView::deleteSelectedNodes()
{
    for (auto *item : m_scene_->selectedItems()) {
        if (auto c = qgraphicsitem_cast<ConnectionGraphicsObject*>(item))
            m_scene_->deleteConnection(c->connection());
    }

    for (auto *item : m_scene_->selectedItems()) {
        if (auto n = qgraphicsitem_cast<NodeGraphicsObject*>(item))
            m_scene_->removeNode(n->node());
    }
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
    for (const auto &cat : m_scene_->registry().categories()) {
        auto *catItem = new QTreeWidgetItem(treeView);
        catItem->setText(0, cat);
        catItem->setData(0, Qt::UserRole, skipText);
        topLevelItems[cat] = catItem;
    }

    for (const auto &assoc : m_scene_->registry().registeredModelsCategoryAssociation()) {
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

    const double d = delta.y() / std::abs(delta.y());
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
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        m_click_pos_ = mapToScene(event->pos());
    }
}

void FlowView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
        // 确保未按下shift
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            QPointF difference = m_click_pos_ - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}
