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
