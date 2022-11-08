#ifndef BMTESTLOADMODEL_H
#define BMTESTLOADMODEL_H

#include <memory>

#include "nodedatamodel.h"

class BmTestLoadModel : public NodeDataModel
{
public:
    BmTestLoadModel();
    ~BmTestLoadModel() = default;

public:
    QString caption() const override { return QString("BmTestLoad"); }
    QString name() const override { return QString("BmTestLoad"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    void setInData(std::shared_ptr<NodeData>, int) override { }
    std::shared_ptr<NodeData> outData(PortIndex port) override;

private:
    QString m_text_;
};

#endif // BMTESTLOADMODEL_H

