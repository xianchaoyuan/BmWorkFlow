#ifndef NODEGRAPHICSOBJECT_H
#define NODEGRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class FlowScene;
class Node;

/**
 * @brief 节点图形对象
 */
class NodeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    NodeGraphicsObject(FlowScene &scene, Node &node);
    virtual ~NodeGraphicsObject();

public:
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    Node &node() { return m_node_; }
    const Node &node() const { return m_node_; }

protected:
    QRectF boundingRect() const override;

    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = nullptr) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    FlowScene &m_scene_;
    Node &m_node_;

    QGraphicsProxyWidget *m_proxy_widget_{};
};

#endif // NODEGRAPHICSOBJECT_H
