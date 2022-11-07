#ifndef NODEDATA_H
#define NODEDATA_H

#include <QString>

/**
 * @brief 节点数据类型
 */
struct NodeDataType
{
    QString id;
    QString name;

    friend bool operator<(const NodeDataType &node1, const NodeDataType &node2) {
        return node1.id < node2.id;
    }

    friend bool operator==(const NodeDataType &node1, const NodeDataType &node2) {
        return node1.id == node2.id;
    }
};

/**
 * @brief 节点数据
 */
class NodeData
{
  public:
    virtual ~NodeData() = default;

    virtual bool sameType(const NodeData &nodeData) const {
        return this->type().id == nodeData.type().id;
    }

    //! 内部使用类型
    virtual NodeDataType type() const = 0;
};

#endif // NODEDATA_H
