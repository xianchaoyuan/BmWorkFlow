#ifndef NODECONNECTIONINTERACTION_H
#define NODECONNECTIONINTERACTION_H

#include "node.h"
#include "connection.h"

class FlowScene;

/**
 * @brief 节点与连接交互
 */
class NodeConnectionInteraction
{
public:
    NodeConnectionInteraction(Node &node,
                              Connection &connection,
                              FlowScene &scene);
    ~NodeConnectionInteraction() = default;

    //! 满足以下条件时可以连接：
    //! 1） 连接“需要”端口
    //! 2） 连接的空闲端位于节点端口上方
    //! 3） 节点端口为空
    //! 4） 连接类型等于节点端口类型，或者存在可以在两者之间转换的注册类型转换
    bool canConnection(PortIndex &portIndex, TypeConverter &converter) const;

    //! 从“canConnect”检查条件
    //! 如果连接是可能的，但需要类型转换，请将转换器节点添加到场景中，并将其正确连接
    //! 将节点分配到连接中所需的端口
    //! 将连接分配给NodeState中的空端口
    //! 调整连接几何图形
    //! 用于启动数据传输的Poke模型
    bool tryConnect() const;

private:
    //! 需要端口类型
    PortType connectionRequiredPort() const;
    QPointF connectionEndScenePosition(PortType portType) const;

    QPointF nodePortScenePosition(PortType portType,
                                  PortIndex portIndex) const;
    PortIndex nodePortIndexUnderScenePoint(PortType portType,
                                           const QPointF &scenePoint) const;
    bool nodePortIsEmpty(PortType portType, PortIndex portIndex) const;

private:
    Node *m_node_{};
    Connection *m_connection_{};
    FlowScene *m_scene_{};
};

#endif // NODECONNECTIONINTERACTION_H
