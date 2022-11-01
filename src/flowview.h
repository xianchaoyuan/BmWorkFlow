#ifndef FLOWVIEW_H
#define FLOWVIEW_H

#include <QGraphicsView>
#include <QContextMenuEvent>

class FlowScene;

/**
 * @brief 流视图
 */
class FlowView : public QGraphicsView
{
    Q_OBJECT

public:
    FlowView(QWidget *parent = nullptr);
    FlowView(FlowScene *scene, QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // FLOWVIEW_H
