#ifndef NODEGEOMETRY_H
#define NODEGEOMETRY_H

#include <memory>

#include <QWidget>
#include <QRectF>
#include <QPointF>
#include <QTransform>
#include <QFontMetrics>

#include "porttype.h"

class NodeDataModel;
class Node;

/**
 * @brief 节点geometry
 */
class NodeGeometry
{
public:
    NodeGeometry(const std::unique_ptr<NodeDataModel> &dataModel);

    unsigned int height() const { return m_height_; }
    void setHeight(unsigned int h) { m_height_ = h; }

    unsigned int width() const { return m_width_; }
    void setWidth(unsigned int w) { m_width_ = w; }

    unsigned int entryHeight() const { return m_entry_height_; }
    void setEntryHeight(unsigned int h) { m_entry_height_ = h; }

    unsigned int entryWidth() const { return m_entry_width_; }
    void setEntryWidth(unsigned int w) { m_entry_width_ = w; }

    unsigned int spacing() const { return m_spacing_; }
    void setSpacing(unsigned int s) { m_spacing_ = s; }

    bool hovered() const { return m_hovered_; }
    void setHovered(unsigned int h) { m_hovered_ = h; }

    unsigned int nSources() const;
    unsigned int nSinks() const;

    QPointF const &draggingPos() const { return m_dragging_pos_; }
    void setDraggingPosition(QPointF const &pos) { m_dragging_pos_ = pos; }

public:
    QRectF entryBoundingRect() const;
    QRectF boundingRect() const;

    //! 无条件更新大小
    void recalculateSize() const;

    //! 字体改变，更新大小
    void recalculateSize(QFont const &font) const;

private:
    //! 标题
    unsigned int captionHeight() const;
    unsigned int captionWidth() const;

    unsigned int portWidth(PortType portType) const;
    unsigned int validationHeight() const;
    unsigned int validationWidth() const;

private:
    mutable unsigned int m_width_;
    mutable unsigned int m_height_;
    mutable unsigned int m_input_port_width_;
    mutable unsigned int m_output_port_width_;
    mutable unsigned int m_entry_width_;
    mutable unsigned int m_entry_height_;
    unsigned int m_spacing_;

    bool m_hovered_;

    unsigned int m_sources_count_;
    unsigned int m_sinks_count_;

    QPointF m_dragging_pos_;

    const std::unique_ptr<NodeDataModel> &m_data_model_;

    mutable QFontMetrics m_font_metrics_;
    mutable QFontMetrics m_bold_font_metrics_;
};

#endif // NODEGEOMETRY_H
