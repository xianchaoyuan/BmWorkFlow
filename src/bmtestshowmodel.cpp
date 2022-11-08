#include "bmtestshowmodel.h"
#include "bmtestdata.h"

#include <QDebug>

BmTestShowModel::BmTestShowModel()
{
}

unsigned int BmTestShowModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType) {
    case PortType::In:
        result = 1;
        break;

    case PortType::Out:
        result = 0;

    default:
        break;
    }

    return result;
}

NodeDataType BmTestShowModel::dataType(PortType portType, PortIndex portIndex) const
{
    return { "text", "T" };
}

void BmTestShowModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex port)
{
    m_node_data_ = nodeData;
    if (m_node_data_) {
        qDebug() << nodeData->type().name;
        auto d = std::dynamic_pointer_cast<BmTestData>(m_node_data_);
        qDebug() << "show model: " << d->text();
    } else {

    }
}

std::shared_ptr<NodeData> BmTestShowModel::outData(PortIndex port)
{
    return m_node_data_;
}
