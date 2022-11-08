#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <QUuid>
#include <QObject>

#include "porttype.h"
#include "connectionstate.h"
#include "connectiongeometry.h"
#include "connectiongraphicsobject.h"
#include "quuidstdhash.h"
#include "typeconverter.h"

class Node;
class NodeData;

/**
 * @brief 连接
 */
class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(PortType portType, Node &node, PortIndex portIndex);
    ~Connection() = default;

    Connection(const Connection &other) = delete;
    Connection operator=(const Connection &other) = delete;

    //! state
    const ConnectionState &connectionState() const;
    ConnectionState &connectionState();

    //! geometry
    ConnectionGeometry &connectionGeometry();
    const ConnectionGeometry &connectionGeometry() const;

    //! graphics object
    ConnectionGraphicsObject &connectionGraphicsObject();
    const ConnectionGraphicsObject &connectionGraphicsObject() const;
    void setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject> &&graphics);

    //! nodes
    bool complete() const {
        return m_in_node_ != nullptr && m_out_node_ != nullptr;
    }
    Node *getNode(PortType portType) const;
    Node* &getNode(PortType portType);
    void setNodeToPort(Node &node, PortType portType, PortIndex portIndex);
    void removeFromNodes() const;
    //! 断开连接侧
    void clearNode(PortType portType);

    void setRequiredPort(PortType portType);
    PortType requiredPort() const;

    PortIndex getPortIndex(PortType portType) const;

    //! 节点数据类型
    NodeDataType dataType(PortType portType) const;

    //! 类型转换器
    void setTypeConverter(TypeConverter converter);

public:
    QUuid id() const { return m_uuid_; }

private:
    QUuid m_uuid_;

    Node *m_out_node_{};
    Node *m_in_node_{};

    PortIndex m_out_port_index_;
    PortIndex m_in_port_index_;

private:
    ConnectionGeometry m_geometry_;
    ConnectionState m_state_;

    std::unique_ptr<ConnectionGraphicsObject> m_graphics_object_;

    TypeConverter m_converter_;
};

#endif // CONNECTION_H
