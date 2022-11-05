#ifndef FLOWSCENE_H
#define FLOWSCENE_H

#include <memory>
#include <unordered_map>

#include <QGraphicsScene>

#include "node.h"
#include "quuidstdhash.h"
#include "datamodelregistry.h"
#include "connection.h"

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
    ~FlowScene();

    DataModelRegistry &registry() const;
    void setRegistry(std::shared_ptr<DataModelRegistry> registry);

    //! 节点创建
    Node &createNode(std::unique_ptr<NodeDataModel> &&dataModel);

    //! 连接创建
    std::shared_ptr<Connection> createConnection(PortType connectedPort,
                                                 Node &node,
                                                 PortIndex portIndex);

public:
    void clearScene();

private:
    using UniqueNode = std::unique_ptr<Node>;
    using SharedConnection = std::shared_ptr<Connection>;

    std::shared_ptr<DataModelRegistry> m_model_registory_;

    std::unordered_map<QUuid, UniqueNode> m_nodes_;
    std::unordered_map<QUuid, SharedConnection> m_connections_;

};

Node *locateNodeAt(QPointF scenePoint, FlowScene &scene,
                   QTransform const &viewTransform);

#endif // FLOWSCENE_H
