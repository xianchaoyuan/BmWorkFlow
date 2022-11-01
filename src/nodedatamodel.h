#ifndef NODEDATAMODEL_H
#define NODEDATAMODEL_H

#include <QObject>

/**
 * @brief 节点数据模型
 */
class NodeDataModel : public QObject
{
    Q_OBJECT

public:
    //! 连接策略
    enum class ConnectionPolicy {
        One,
        Many,
    };

    NodeDataModel();
    virtual ~NodeDataModel() = default;

    //! 说明
    virtual QString caption() const = 0;
    virtual bool captionVisible() const { return true; }

    //! 独一无二的名字（不能重复）
    virtual QString name() const = 0;
};

#endif // NODEDATAMODEL_H
