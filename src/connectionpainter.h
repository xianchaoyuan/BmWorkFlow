#ifndef CONNECTIONPAINTER_H
#define CONNECTIONPAINTER_H

#include <QPainter>

class ConnectionGeometry;
class ConnectionState;
class Connection;

/**
 * @brief 连接线绘制
 */
class ConnectionPainter
{
public:
    static void paint(QPainter *painter, Connection const &connection);
    static QPainterPath getPainterStroke(ConnectionGeometry const &geom);
};

#endif // CONNECTIONPAINTER_H
