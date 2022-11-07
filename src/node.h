#ifndef NODE_H
#define NODE_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QDebug>

#include "nodestate.h"
#include "nodegeometry.h"

class NodeDataModel;
class NodeGraphicsObject;

/**
 * @brief 节点
 */
class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(std::unique_ptr<NodeDataModel> &&dataModel);
    ~Node() = default;

    const QUuid &id() const;

    NodeGeometry &nodeGeometry();
    const NodeGeometry &nodeGeometry() const;

    NodeGraphicsObject &nodeGraphicsObject();
    const NodeGraphicsObject &nodeGraphicsObject() const;
    void setGraphicsObject(std::unique_ptr<NodeGraphicsObject> &&graphics);

    NodeState &nodeState();
    const NodeState &nodeState() const;

    NodeDataModel *nodeDataModel() const;

public slots:
    //! 从模型的out索引端口获取数据并将其传播到连接
    void onDataUpdated(PortIndex index);

private:
    //! 声明顺序不可改变
    std::unique_ptr<NodeDataModel> m_data_model_;
    std::unique_ptr<NodeGraphicsObject> m_graphics_object_;

    QUuid m_uuid_;
    NodeState m_state_;
    NodeGeometry m_geometry_;
};

#endif // NODE_H
