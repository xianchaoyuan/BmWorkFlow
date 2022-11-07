#include "ImageShowModel.h"

#include <QEvent>
#include <QDir>
#include <QFileDialog>

#include "datamodelregistry.h"
#include "PixmapData.h"

ImageShowModel::ImageShowModel()
    : _label(new QLabel("Image will appear here"))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    QFont f = _label->font();
    f.setBold(true);
    f.setItalic(true);

    _label->setFont(f);
    _label->setFixedSize(200, 200);
    _label->installEventFilter(this);
}

unsigned int ImageShowModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType) {
    case PortType::In:
        result = 1;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}

bool ImageShowModel::eventFilter(QObject *object, QEvent *event)
{
    if (object == _label) {
        int w = _label->width();
        int h = _label->height();

        if (event->type() == QEvent::Resize) {
            auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);
            if (d) {
                _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
            }
        }
    }

    return false;
}

NodeDataType ImageShowModel::dataType(PortType, PortIndex) const
{
    return PixmapData().type();
}

std::shared_ptr<NodeData> ImageShowModel::outData(PortIndex port)
{
    return _nodeData;
}
