#include "connection.h"
#include "node.h"
#include "nodegraphicsobject.h"

Connection::Connection(PortType portType, Node &node, PortIndex portIndex)
    : m_uuid_(QUuid::createUuid()),
      m_in_port_index_(INVALID),
      m_out_port_index_(INVALID),
      m_state_()
{
    setNodeToPort(node, portType, portIndex);
    setRequiredPort(oppositePort(portType));
}

ConnectionGeometry &Connection::connectionGeometry()
{
    return m_geometry_;
}

const ConnectionGeometry &Connection::connectionGeometry() const
{
    return m_geometry_;
}

ConnectionGraphicsObject &Connection::connectionGraphicsObject()
{
    return *m_graphics_object_;
}

const ConnectionGraphicsObject &Connection::connectionGraphicsObject() const
{
    return *m_graphics_object_;
}

void Connection::setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject> &&graphics)
{
    m_graphics_object_ = std::move(graphics);

    if (requiredPort() != PortType::None) {

        PortType attachedPort = oppositePort(requiredPort());
        PortIndex attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform =
                node->nodeGraphicsObject().sceneTransform();
        QPointF pos = node->nodeGeometry().portScenePosition(attachedPortIndex,
                                                             attachedPort,
                                                             nodeSceneTransform);

        m_graphics_object_->setPos(pos);
    }

    m_graphics_object_->move();
}

Node *Connection::getNode(PortType portType) const
{
    switch (portType)
    {
    case PortType::In:
        return m_in_node_;
        break;

    case PortType::Out:
        return m_out_node_;
        break;

    default:
        // not possible
        break;
    }
    return nullptr;
}

Node *&Connection::getNode(PortType portType)
{
    switch (portType)
    {
    case PortType::In:
        return m_in_node_;
        break;

    case PortType::Out:
        return m_out_node_;
        break;

    default:
        // not possible
        break;
    }
    Q_UNREACHABLE();
}

PortIndex Connection::getPortIndex(PortType portType) const
{
    PortIndex result = INVALID;
    switch (portType)
    {
    case PortType::In:
        result = m_in_port_index_;
        break;

    case PortType::Out:
        result = m_out_port_index_;

        break;

    default:
        break;
    }

    return result;
}

void Connection::setRequiredPort(PortType portType)
{
    m_state_.setRequiredPort(portType);

    switch (portType)
    {
    case PortType::Out:
        m_out_node_  = nullptr;
        m_out_port_index_ = INVALID;
        break;

    case PortType::In:
        m_in_node_  = nullptr;
        m_in_port_index_ = INVALID;
        break;

    default:
        break;
    }
}

PortType Connection::requiredPort() const
{
    return m_state_.requiredPort();
}

bool Connection::complete() const
{
    return m_in_node_ != nullptr && m_out_node_ != nullptr;
}

void Connection::setNodeToPort(Node &node, PortType portType, PortIndex portIndex)
{
    auto &nodeWeak = getNode(portType);
    nodeWeak = &node;

    if (portType == PortType::Out)
        m_out_port_index_ = portIndex;
    else
        m_in_port_index_ = portIndex;
}

