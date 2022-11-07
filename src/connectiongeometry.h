#ifndef CONNECTIONGEOMETRY_H
#define CONNECTIONGEOMETRY_H

#include <QPointF>
#include <QRectF>

#include "porttype.h"

/**
 * @brief 连接线geometry
 */
class ConnectionGeometry
{
public:
    ConnectionGeometry();

    //! 获取补充点
    std::pair<QPointF, QPointF> pointsC1C2() const;
    QRectF boundingRect() const;

    const QPointF &getEndPoint(PortType portType) const;
    void setEndPoint(PortType portType, const QPointF &point);
    void moveEndPoint(PortType portType, const QPointF &offset);

    QPointF source() const { return m_out_; }
    QPointF sink() const { return m_in_; }

    bool hovered() const { return m_hovered_; }
    void setHovered(bool hovered) { m_hovered_ = hovered; }

    double lineWidth() const { return m_line_width_; }
    void setLineWidth(double width) { m_line_width_ = width; }

private:
    QPointF m_in_;
    QPointF m_out_;

    double m_line_width_;
    bool m_hovered_;
};

#endif // CONNECTIONGEOMETRY_H
