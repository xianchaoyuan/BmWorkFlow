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
    explicit FlowView(FlowScene *scene, QWidget *parent = nullptr);
    FlowView(QWidget *parent = nullptr);
    FlowView(const FlowView &) = delete;
    FlowView operator=(const FlowView &) = delete;

public slots:
    void zoomIn();
    void zoomOut();
    void zoomAll();
    void zoomToRect(const QRectF &rect);

    void deleteSelectedNodes();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    FlowScene *m_scene_{};

    QAction *m_act_clear_selection_{};
    QAction *m_act_delete_selection_{};

    QPointF m_click_pos_;
};

#endif // FLOWVIEW_H
