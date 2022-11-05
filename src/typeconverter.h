#ifndef TYPECONVERTER_H
#define TYPECONVERTER_H

#include <functional>

#include <QHash>

#include "nodedata.h"
#include "memory.h"

using SharedNodeData = std::shared_ptr<NodeData>;

// 接收节点数据并返回节点数据的函数
using TypeConverter =
  std::function<SharedNodeData(SharedNodeData)>;

// data-type-in, data-type-out
using TypeConverterId =
  std::pair<NodeDataType, NodeDataType>;

struct TypeConverterIdHash
{
    std::size_t operator()(const TypeConverterId& converter) const
    {
        return qHash(converter.first.id)
            ^ qHash(converter.second.id);
    }
};

#endif // TYPECONVERTER_H
