#pragma once

#include <iostream>

#include <QObject>
#include <QLabel>

#include "nodedatamodel.h"

/**
 * @brief 图片显示模型
 */
class ImageShowModel : public NodeDataModel
{
    Q_OBJECT

public:
    ImageShowModel();
    virtual ~ImageShowModel() {}

public:
    QString caption() const override { return QString("Image Display"); }
    QString name() const override { return QString("ImageShowModel"); }

public:
    virtual QString modelName() const { return QString("Resulting Image"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *_label;
    std::shared_ptr<NodeData> _nodeData;
};
