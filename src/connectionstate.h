#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include <QUuid>

#include "porttype.h"

class QPointF;
class Node;

/**
 * @brief 连接状态数据
 */
class ConnectionState
{
public:
    explicit ConnectionState(PortType port = PortType::None) : m_required_port_(port) {}
    ConnectionState(const ConnectionState &) = delete;
    ConnectionState operator=(const ConnectionState &) = delete;
    ~ConnectionState();

public:
    //! 需要端口
    void setRequiredPort(PortType end) { m_required_port_ = end; }
    PortType requiredPort() const { return m_required_port_; }

    //! 是否需要连接端口
    bool requiresPort() const { return m_required_port_ != PortType::None; }

private:
    PortType m_required_port_;
};

#endif // CONNECTIONSTATE_H
