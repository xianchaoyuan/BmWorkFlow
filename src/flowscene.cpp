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

    auto nodePtr = node.get();
    return *nodePtr;
}
