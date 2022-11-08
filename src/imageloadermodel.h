#ifndef IMAGELOADERMODEL_H
#define IMAGELOADERMODEL_H

#include <iostream>

#include <QObject>
#include <QLabel>

#include "nodedatamodel.h"

/**
 * @brief 图片加载模型
 */
class ImageLoaderModel : public NodeDataModel
{
    Q_OBJECT

public:
    ImageLoaderModel();
    virtual ~ImageLoaderModel() {}

public:
    QString caption() const override { return QString("Image Source"); }
    QString name() const override { return QString("ImageLoaderModel"); }

public:
    virtual QString modelName() const { return QString("Source Image"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *_label;
    QPixmap _pixmap;
};

#endif // IMAGELOADERMODEL_H
