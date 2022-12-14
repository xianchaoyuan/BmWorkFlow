#ifndef NODESTATE_H
#define NODESTATE_H

#include <memory>
#include <vector>
#include <unordered_map>

#include <QUuid>

#include "porttype.h"
#include "memory.h"
#include "connection.h"

class NodeDataModel;

/**
 * @brief 节点状态
 */
class NodeState
{
public:
    NodeState(const std::unique_ptr<NodeDataModel> &model);

    using ConnectionPtrSet =
    std::unordered_map<QUuid, Connection *>;

    const std::vector<ConnectionPtrSet> &getEntries(PortType portType) const;
    std::vector<ConnectionPtrSet> &getEntries(PortType portType);

    ConnectionPtrSet connections(PortType portType, PortIndex portIndex) const;
    void setConnection(PortType portType, PortIndex portIndex, Connection &connection);
    void eraseConnection(PortType portType, PortIndex portIndex, QUuid id);

private:
    std::vector<ConnectionPtrSet> m_in_connections_;
    std::vector<ConnectionPtrSet> m_out_connections_;
};

#endif // NODESTATE_H
