#ifndef CONNECTIONGRAPHICSOBJECT_H
#define CONNECTIONGRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>

class FlowScene;
class Connection;
class ConnectionGeometry;

/**
 * @brief 连接线绘制
 */
class ConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    ConnectionGraphicsObject(FlowScene &scene, Connection &connection);
    ~ConnectionGraphicsObject() override;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

public:
    //! 更新两端位置
    void move();
    Connection &connection() const { return m_connection_; }

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void addGraphicsEffect();

private:
    FlowScene &m_scene_;
    Connection &m_connection_;
};

#endif // CONNECTIONGRAPHICSOBJECT_H
