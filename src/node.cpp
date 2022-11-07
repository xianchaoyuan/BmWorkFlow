#include "node.h"
#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "nodegraphicsobject.h"

Node::Node(std::unique_ptr<NodeDataModel> &&dataModel)
    : m_uuid_(QUuid::createUuid()),
      m_data_model_(std::move(dataModel)),
      m_state_(m_data_model_),
      m_geometry_(m_data_model_),
      m_graphics_object_(nullptr)
{
    m_geometry_.recalculateSize();
}

const QUuid &Node::id() const
{
    return m_uuid_;
}

NodeGeometry &Node::nodeGeometry()
{
    return m_geometry_;
}

const NodeGeometry &Node::nodeGeometry() const
{
    return m_geometry_;
}

NodeGraphicsObject &Node::nodeGraphicsObject()
{
    return *m_graphics_object_.get();
}

const NodeGraphicsObject &Node::nodeGraphicsObject() const
{
    return *m_graphics_object_.get();
}

void Node::setGraphicsObject(std::unique_ptr<NodeGraphicsObject> &&graphics)
{
    m_graphics_object_ = std::move(graphics);
    m_geometry_.recalculateSize();
}

NodeState &Node::nodeState()
{
    return m_state_;
}

const NodeState &Node::nodeState() const
{
    return m_state_;
}

NodeDataModel *Node::nodeDataModel() const
{
    return m_data_model_.get();
}

void Node::onDataUpdated(PortIndex index)
{
    auto nodeData = m_data_model_->outData(index);

    const auto &connections =
            m_state_.connections(PortType::Out, index);

//    for (const auto &c : connections)
//        c.second->propagateData(nodeData);
}
