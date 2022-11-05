#ifndef NODEPAINTER_H
#define NODEPAINTER_H

#include <QPainter>

class Node;
class FlowScene;
class NodeState;
class NodeGeometry;
class NodeDataModel;
class NodeGraphicsObject;

/**
 * @brief 节点画家
 */
class NodePainter
{
public:
    NodePainter() = default;

    static void paint(QPainter *painter, Node &node, FlowScene const &scene);

    //! 边框
    static void drawNodeRect(QPainter *painter,
                             const NodeGeometry &geom,
                             const NodeDataModel *model,
                             const NodeGraphicsObject &graphicsObject);

    //! 模块说明
    static void drawNodeCaption(QPainter *painter,
                              const NodeGeometry &geom,
                              const NodeDataModel *model);

    //! 条目标签
    static void drawEntryLabels(QPainter *painter,
                                const NodeGeometry &geom,
                                const NodeDataModel *model);

    //! 连接点
    static void drawConnectionPoints(QPainter *painter,
                                     const NodeGeometry &geom,
                                     const NodeState &state,
                                     const NodeDataModel *model,
                                     const FlowScene &scene);

    //! 实心连接点
    static void drawFilledConnectionPoints(QPainter *painter,
                                           const NodeGeometry &geom,
                                           const NodeState &state,
                                           const NodeDataModel *model);

    //! resize边框
    static void drawResizeRect(QPainter *painter,
                               const NodeGeometry &geom,
                               const NodeDataModel *model);
};

#endif // NODEPAINTER_H
