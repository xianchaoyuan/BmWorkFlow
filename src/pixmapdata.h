#pragma once

#include <QPixmap>

#include "nodedata.h"

/**
 * @brief 可能包含需要在节点编辑器图形中传输的任何用户数据
 */
class PixmapData : public NodeData
{
public:
    PixmapData() {}
    PixmapData(QPixmap const &pixmap)
        : _pixmap(pixmap) {}

    NodeDataType type() const override
    {
        return { "pixmap", "P" };
    }
    QPixmap pixmap() const { return _pixmap; }

private:
    QPixmap _pixmap;
};
