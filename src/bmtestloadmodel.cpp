#include "bmtestloadmodel.h"
#include "bmtestdata.h"

#include <QTime>

BmTestLoadModel::BmTestLoadModel()
{
    m_text_ = "LoadModel" + QTime::currentTime().toString();
}

unsigned int BmTestLoadModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType) {
    case PortType::In:
        result = 0;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}

NodeDataType BmTestLoadModel::dataType(PortType portType, PortIndex portIndex) const
{
    return { "text", "T" };
}

std::shared_ptr<NodeData> BmTestLoadModel::outData(PortIndex port)
{
    return std::make_shared<BmTestData>(m_text_);
}
