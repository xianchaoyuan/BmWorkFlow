#ifndef BMTESTDATA_H
#define BMTESTDATA_H

#include "nodedata.h"

class BmTestData : public NodeData
{
public:
    BmTestData() {}
    BmTestData(const QString &text)
        : m_text_(text) {}

    NodeDataType type() const override
    {
        return { "text", "T" };
    }
    QString text() const { return m_text_; }

private:
    QString m_text_;
};

#endif // BMTESTDATA_H
