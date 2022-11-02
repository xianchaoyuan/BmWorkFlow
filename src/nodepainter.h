#ifndef NODEPAINTER_H
#define NODEPAINTER_H

#include <QPainter>

class Node;
class FlowScene;
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
    static void drawNodeRect(QPainter *painter,
                             NodeGeometry const &geom,
                             NodeDataModel const *model,
                             NodeGraphicsObject const &graphicsObject);

    static void drawModelName(QPainter *painter,
                              NodeGeometry const &geom,
                              NodeDataModel const *model);

    static void drawEntryLabels(QPainter *painter,
                                NodeGeometry const &geom,
                                NodeDataModel const *model);

    static void drawConnectionPoints(QPainter *painter,
                                     NodeGeometry const &geom,
                                     NodeDataModel const *model,
                                     FlowScene const &scene);

    static void drawFilledConnectionPoints(QPainter *painter,
                                           NodeGeometry const &geom,
                                           NodeDataModel const *model);

    static void drawResizeRect(QPainter *painter,
                               NodeGeometry const &geom,
                               NodeDataModel const *model);

    static void drawValidationRect(QPainter *painter,
                                   NodeGeometry const & geom,
                                   NodeDataModel const *model,
                                   NodeGraphicsObject const &graphicsObject);
};

#endif // NODEPAINTER_H
