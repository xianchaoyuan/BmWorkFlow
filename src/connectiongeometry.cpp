#include "connectiongeometry.h"

ConnectionGeometry::ConnectionGeometry()
    : m_in_(0, 0),
      m_out_(0, 0),
      m_line_width_(3.0),
      m_hovered_(false)
{

}

std::pair<QPointF, QPointF> ConnectionGeometry::pointsC1C2() const
{
    const double defaultOffset = 200;
    double xDistance = m_in_.x() - m_out_.x();
    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));
    double verticalOffset = 0;
    double ratioX = 0.5;

    if (xDistance <= 0) {
        double yDistance = m_in_.y() - m_out_.y() + 20;
        double vector = yDistance < 0 ? -1.0 : 1.0;
        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;
        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(m_out_.x() + horizontalOffset,
               m_out_.y() + verticalOffset);
    QPointF c2(m_in_.x() - horizontalOffset,
               m_in_.y() - verticalOffset);

    return std::make_pair(c1, c2);
}

QRectF ConnectionGeometry::boundingRect() const
{
    auto points = pointsC1C2();
    QRectF basicRect = QRectF(m_out_, m_in_).normalized();
    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    const float diam = 8;
    QRectF commonRect = basicRect.united(c1c2Rect);
    QPointF const cornerOffset(diam, diam);

    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}

const QPointF &ConnectionGeometry::getEndPoint(PortType portType) const
{
    Q_ASSERT(portType != PortType::None);
    return (portType == PortType::Out ?
                m_out_ :
                m_in_);
}

void ConnectionGeometry::setEndPoint(PortType portType, const QPointF &point)
{
    switch (portType)
    {
    case PortType::Out:
        m_out_ = point;
        break;

    case PortType::In:
        m_in_ = point;
        break;

    default:
        break;
    }
}

void ConnectionGeometry::moveEndPoint(PortType portType, const QPointF &offset)
{
    switch (portType)
    {
    case PortType::Out:
        m_out_ += offset;
        break;

    case PortType::In:
        m_in_ += offset;
        break;

    default:
        break;
    }
}
