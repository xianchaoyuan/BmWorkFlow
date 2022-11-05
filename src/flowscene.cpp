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

std::shared_ptr<Connection> FlowScene::createConnection(PortType connectedPort, Node &node, PortIndex portIndex)
{
    auto connection = std::make_shared<Connection>(connectedPort, node, portIndex);
    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *connection);

    connection->setGraphicsObject(std::move(cgo));
    m_connections_[connection->id()] = connection;

    return connection;
}

void FlowScene::clearScene()
{
    while (m_connections_.size() > 0) {

    }

    while (m_nodes_.size() > 0) {

    }
}

Node *locateNodeAt(QPointF scenePoint, FlowScene &scene, const QTransform &viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items =
            scene.items(scenePoint,
                        Qt::IntersectsItemShape,
                        Qt::DescendingOrder,
                        viewTransform);

    //! items convertable to NodeGraphicsObject
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
