#include "nodestate.h"
#include "nodedatamodel.h"
#include "connection.h"

NodeState::NodeState(const std::unique_ptr<NodeDataModel> &model)
    : m_in_connections_(model->nPorts(PortType::In)),
      m_out_connections_(model->nPorts(PortType::Out)),
      m_resizing_(false)
{

}

const std::vector<NodeState::ConnectionPtrSet> &NodeState::getEntries(PortType portType) const
{
    if (portType == PortType::In)
        return m_in_connections_;
    else
        return m_out_connections_;
}

std::vector<NodeState::ConnectionPtrSet> &NodeState::getEntries(PortType portType)
{
    if (portType == PortType::In)
        return m_in_connections_;
    else
        return m_out_connections_;
}

NodeState::ConnectionPtrSet NodeState::connections(PortType portType, PortIndex portIndex) const
{
    const auto &connections = getEntries(portType);
    return connections[portIndex];
}

void NodeState::setConnection(PortType portType, PortIndex portIndex, Connection &connection)
{
    auto &connections = getEntries(portType);
    connections.at(portIndex).insert(std::make_pair(connection.id(),
                                                    &connection));
}

void NodeState::eraseConnection(PortType portType, PortIndex portIndex, QUuid id)
{
    getEntries(portType)[portIndex].erase(id);
}

void NodeState::setResizing(bool resizing)
{
    m_resizing_ = resizing;
}

bool NodeState::resizing() const
{
    return m_resizing_;
}
