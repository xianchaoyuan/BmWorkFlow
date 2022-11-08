#ifndef NODEDATAMODEL_H
#define NODEDATAMODEL_H

#include <memory>

#include <QObject>

#include "nodedata.h"
#include "porttype.h"

//! 节点验证状态
enum class NodeValidationState
{
    Valid,
    Warning,
    Error
};

/**
 * @brief 节点数据模型
 */
class NodeDataModel : public QObject
{
    Q_OBJECT

public:
    NodeDataModel();
    virtual ~NodeDataModel() = default;

    //! 说明
    virtual QString caption() const = 0;
    virtual bool captionVisible() const { return true; }

    //! 端口标题在GUI中用于标记各个端口
    virtual QString portCaption(PortType, PortIndex) const { return QString(); }
    virtual bool portCaptionVisible(PortType, PortIndex) const { return false; }

    //! 获取输出数据
    virtual std::shared_ptr<NodeData> outData(PortIndex port) = 0;

    //! 独一无二的名字（不能重复）
    virtual QString name() const = 0;

    virtual QString validationMessage() const { return QString(""); }
    virtual NodeValidationState validationState() const { return NodeValidationState::Valid; }

public:
    //! 连接策略
    enum class ConnectionPolicy {
        One,
        Many,
    };

    virtual ConnectionPolicy portOutConnectionPolicy(PortIndex) const {
        return ConnectionPolicy::Many;
    }

    virtual ConnectionPolicy portInConnectionPolicy(PortIndex) const {
        return ConnectionPolicy::One;
    }

public:
    virtual unsigned int nPorts(PortType portType) const = 0;
    virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

};

#endif // NODEDATAMODEL_H
