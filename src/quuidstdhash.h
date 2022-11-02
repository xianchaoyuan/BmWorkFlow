#pragma once

#include <functional>

#include <QUuid>
#include <QVariant>

namespace std
{
template<>
struct hash<QUuid>
{
    inline std::size_t operator()(QUuid const &uid) const
    {
        return qHash(uid);
    }
};
}

