#include "flowscene.h"
#include "node.h"
#include "nodegraphicsobject.h"

FlowScene::FlowScene(std::shared_ptr<DataModelRegistry> registory, QObject *parent)
    : QGraphicsScene(parent)
{
    m_model_registory_ = registory;
}

FlowScene::FlowScene(QObject *parent)
    : FlowScene(std::make_shared<DataModelRegistry>(), parent)
{

}

FlowScene::~FlowScene()
{
    clearScene();
}

DataModelRegistry &FlowScene::registry() const
{
    return *m_model_registory_;
}

void FlowScene::setRegistry(std::shared_ptr<DataModelRegistry> registry)
{
    m_model_registory_ = std::move(registry);
}

Node &FlowScene::createNode(std::unique_ptr<NodeDataModel> &&dataModel)
{
    auto node = std::make_unique<Node>(std::move(dataModel));
    auto ngo  = std::make_unique<NodeGraphicsObject>(*this, *node);

    node->setGraphicsObject(std::move(ngo));

    auto *nodePtr = node.get();
    m_nodes_[node->id()] = std::move(node);
    return *nodePtr;
}

void FlowScene::removeNode(Node &node)
{
    for(auto portType: { PortType::In, PortType::Out }) {
        auto nodeState = node.nodeState();
        const auto &nodeEntries = nodeState.getEntries(portType);
        for (auto &connections : nodeEntries) {
            for (const auto &pair : connections)
                deleteConnection(*pair.second);
        }
    }

    m_nodes_.erase(node.id());
}

std::shared_ptr<Connection> FlowScene::createConnection(PortType connectedPort, Node &node, PortIndex portIndex)
{
    auto connection = std::make_shared<Connection>(connectedPort, node, portIndex);
    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *connection);

    connection->setGraphicsObject(std::move(cgo));
    m_connections_[connection->id()] = connection;

    return connection;
}

void FlowScene::deleteConnection(const Connection &connection)
{
    auto it = m_connections_.find(connection.id());
    if (it != m_connections_.end()) {
        connection.removeFromNodes();
        m_connections_.erase(it);
    }
}

void FlowScene::clearScene()
{
    while (m_connections_.size() > 0) {
        deleteConnection(*m_connections_.begin()->second);
    }

    while (m_nodes_.size() > 0) {
        removeNode(*m_nodes_.begin()->second);
    }
}

Node *locateNodeAt(QPointF scenePoint, FlowScene &scene, const QTransform &viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items = scene.items(scenePoint,
                                              Qt::IntersectsItemShape,
                                              Qt::DescendingOrder,
                                              viewTransform);

    // 可转换为NodeGraphicsObject的项
    std::vector<QGraphicsItem*> filteredItems;
    std::copy_if(items.begin(),
                 items.end(),
                 std::back_inserter(filteredItems),
                 [] (QGraphicsItem *item)
    {
        return (dynamic_cast<NodeGraphicsObject*>(item) != nullptr);
    });

    Node *resultNode = nullptr;

    if (!filteredItems.empty()) {
        QGraphicsItem* graphicsItem = filteredItems.front();
        auto ngo = dynamic_cast<NodeGraphicsObject*>(graphicsItem);

        resultNode = &ngo->node();
    }

    return resultNode;
}
