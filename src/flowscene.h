#ifndef FLOWSCENE_H
#define FLOWSCENE_H

#include <memory>

#include <QGraphicsScene>

#include "datamodelregistry.h"

class Node;
class NodeGraphicsObject;

/**
 * @brief 流场景
 */
class FlowScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit FlowScene(std::shared_ptr<DataModelRegistry> registory, QObject *parent = nullptr);
    FlowScene(QObject *parent = nullptr);
    ~FlowScene() = default;

    DataModelRegistry &registry() const;
    void setRegistry(std::shared_ptr<DataModelRegistry> registry);

    Node &createNode(std::unique_ptr<NodeDataModel> &&dataModel);

private:
    std::shared_ptr<DataModelRegistry> m_model_registory_;
};

#endif // FLOWSCENE_H
