#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "node.h"

#include <cmath>

NodeGeometry::NodeGeometry(const std::unique_ptr<NodeDataModel> &dataModel)
    : m_width_(200),
      m_height_(100),
      m_input_port_width_(70),
      m_output_port_width_(70),
      m_entry_height_(20),
      m_spacing_(20),
      m_hovered_(false),
      m_sources_count_(dataModel->nPorts(PortType::Out)),
      m_sinks_count_(dataModel->nPorts(PortType::In)),
      m_dragging_pos_(-1000, -1000),
      m_data_model_(dataModel),
      m_font_metrics_(QFont()),
      m_bold_font_metrics_(QFont())
{

}

QRectF NodeGeometry::boundingRect() const
{
    double addon = 4 * 4.0;
    return QRectF(0 - addon,
                  0 - addon,
                  m_width_ + 2 * addon,
                  m_height_ + 2 * addon);
}

QRect NodeGeometry::resizeRect() const
{
    unsigned int rectSize = 7;
    return QRect(m_width_ - rectSize,
                 m_height_ - rectSize,
                 rectSize,
                 rectSize);
}

void NodeGeometry::recalculateSize() const
{
    // height
    m_entry_height_ = m_font_metrics_.height();

    {
        unsigned int maxNumOfEntries = std::max(m_sinks_count_, m_sources_count_);
        unsigned int step = m_entry_height_ + m_spacing_;
        m_height_ = step * maxNumOfEntries;
    }

    if (auto w = m_data_model_->embeddedWidget()) {
        m_height_ = std::max(m_height_, static_cast<unsigned>(w->height()));
    }
    m_height_ += captionHeight();

    // width
    m_input_port_width_  = portWidth(PortType::In);
    m_output_port_width_ = portWidth(PortType::Out);

    m_width_ = m_input_port_width_ +
            m_output_port_width_ +
            2 * m_spacing_;

    if (auto w = m_data_model_->embeddedWidget()) {
        m_width_ += w->width();
    }
    m_width_ = std::max(m_width_, captionWidth());

    // validation
    if (m_data_model_->validationState() != NodeValidationState::Valid) {
        m_width_   = std::max(m_width_, validationWidth());
        m_height_ += validationHeight() + m_spacing_;
    }
}

void NodeGeometry::recalculateSize(const QFont &font) const
{
    QFontMetrics fontMetrics(font);
    QFont boldFont = font;
    boldFont.setBold(true);

    QFontMetrics boldFontMetrics(boldFont);
    if (m_bold_font_metrics_ != boldFontMetrics) {
        m_font_metrics_      = fontMetrics;
        m_bold_font_metrics_ = boldFontMetrics;

        recalculateSize();
    }
}

QPointF NodeGeometry::portScenePosition(PortIndex index, PortType portType, const QTransform &t) const
{
    unsigned int step = m_entry_height_ + m_spacing_;

    QPointF result;

    double totalHeight = 0.0;
    totalHeight += captionHeight();
    totalHeight += step * index;

    switch (portType) {
    case PortType::Out: {
        double x = m_width_;
        result = QPointF(x, totalHeight);
        break;
    }
    case PortType::In: {
        double x = 0.0;
        result = QPointF(x, totalHeight);
        break;
    }
    default:
        break;
    }

    return t.map(result);
}

PortIndex NodeGeometry::checkHitScenePoint(PortType portType, const QPointF &scenePoint, const QTransform &sceneTransform) const
{
    PortIndex result = INVALID;

    if (portType == PortType::None)
        return result;

    const double tolerance = 2.0 * 8.0;
    unsigned int const nItems = m_data_model_->nPorts(portType);

    for (unsigned int i = 0; i < nItems; ++i) {
        auto pp = portScenePosition(i, portType, sceneTransform);

        QPointF p = pp - scenePoint;
        auto    distance = std::sqrt(QPointF::dotProduct(p, p));

        if (distance < tolerance) {
            result = PortIndex(i);
            break;
        }
    }

    return result;
}

unsigned int NodeGeometry::captionHeight() const
{
    if (!m_data_model_->captionVisible())
        return 0;

    QString name = m_data_model_->caption();
    return m_bold_font_metrics_.boundingRect(name).height();
}

unsigned int NodeGeometry::captionWidth() const
{
    if (!m_data_model_->captionVisible())
        return 0;

    QString name = m_data_model_->caption();
    return m_bold_font_metrics_.boundingRect(name).width();
}

unsigned int NodeGeometry::portWidth(PortType portType) const
{
    unsigned width = 0;

    for (auto i = 0; i < m_data_model_->nPorts(portType); ++i) {
        QString name;

        if (m_data_model_->portCaptionVisible(portType, i)) {
            name = m_data_model_->portCaption(portType, i);
        } else {
            name = m_data_model_->dataType(portType, i).name;
        }

        width = std::max(unsigned(m_font_metrics_.horizontalAdvance(name)), width);
    }

    return width;
}

unsigned int NodeGeometry::validationHeight() const
{
    QString msg = m_data_model_->validationMessage();
    return m_bold_font_metrics_.boundingRect(msg).height();
}

unsigned int NodeGeometry::validationWidth() const
{
    QString msg = m_data_model_->validationMessage();
    return m_bold_font_metrics_.boundingRect(msg).width();
}
