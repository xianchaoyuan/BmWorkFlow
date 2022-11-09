#ifndef BMTESTSHOWMODEL_H
#define BMTESTSHOWMODEL_H

#include <memory>

#include "nodedatamodel.h"

class BmTestShowModel : public NodeDataModel
{
public:
    BmTestShowModel();
    ~BmTestShowModel() = default;

public:
    QString caption() const override { return QString("BmTestShow"); }
    QString name() const override { return QString("BmTestShow"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    void setInData(std::shared_ptr<NodeData>, int) override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;

    QString getTestText() const override { return m_text_; }
    QPixmap backgroundPixmap() const override { return m_bkg_pixmap_; }

    QWidget *propertyWidget() const override { return m_property_widget_; }

private:
    std::shared_ptr<NodeData> m_node_data_;

    QString m_text_;
    QPixmap m_bkg_pixmap_;

    QWidget *m_property_widget_{};
};

#endif // BMTESTSHOWMODEL_H
