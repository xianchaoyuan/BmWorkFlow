#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "node.h"

NodeGeometry::NodeGeometry(const std::unique_ptr<NodeDataModel> &dataModel)
    : m_width_(100),
      m_height_(150),
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

QRectF NodeGeometry::entryBoundingRect() const
{
    double const addon = 0.0;
    return QRectF(0 - addon,
                  0 - addon,
                  m_entry_width_ + 2 * addon,
                  m_entry_height_ + 2 * addon);
}

QRectF NodeGeometry::boundingRect() const
{
    double addon = 0.0;
    return QRectF(0 - addon,
                  0 - addon,
                  m_width_ + 2 * addon,
                  m_height_ + 2 * addon);
}

void NodeGeometry::recalculateSize() const
{
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

    m_input_port_width_  = portWidth(PortType::In);
    m_output_port_width_ = portWidth(PortType::Out);

    m_width_ = m_input_port_width_ +
            m_output_port_width_ +
            2 * m_spacing_;

    if (auto w = m_data_model_->embeddedWidget()) {
        m_width_ += w->width();
    }

    m_width_ = std::max(m_width_, captionWidth());

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
        m_font_metrics_     = fontMetrics;
        m_bold_font_metrics_ = boldFontMetrics;

        recalculateSize();
    }
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        width = std::max(unsigned(m_font_metrics_.horizontalAdvance(name)),
                         width);
#else
        width = std::max(unsigned(m_font_metrics_.width(name)),
                         width);
#endif
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
