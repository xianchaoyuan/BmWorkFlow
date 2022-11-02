#ifndef FLOWSCENE_H
#define FLOWSCENE_H

#include <memory>
#include <unordered_map>

#include <QGraphicsScene>

#include "node.h"
#include "quuidstdhash.h"
#include "datamodelregistry.h"

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
    using UniqueNode = std::unique_ptr<Node>;

    std::shared_ptr<DataModelRegistry> m_model_registory_;

    std::unordered_map<QUuid, UniqueNode> m_nodes_;

};

#endif // FLOWSCENE_H
