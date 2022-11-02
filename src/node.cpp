#include "node.h"
#include "nodegeometry.h"
#include "nodedatamodel.h"
#include "nodegraphicsobject.h"

Node::Node(std::unique_ptr<NodeDataModel> &&dataModel)
    : m_uuid_(QUuid::createUuid()),
      m_data_model_(std::move(dataModel)),
      m_geometry_(m_data_model_),
      m_graphics_object_(nullptr)
{

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
}

NodeDataModel *Node::nodeDataModel() const
{
    return m_data_model_.get();
}
