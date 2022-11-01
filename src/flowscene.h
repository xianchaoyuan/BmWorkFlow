#ifndef FLOWSCENE_H
#define FLOWSCENE_H

#include <memory>

#include <QGraphicsScene>

#include "datamodelregistry.h"

/**
 * @brief 流场景
 */
class FlowScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit FlowScene(std::shared_ptr<DataModelRegistry> registory, QObject *parent = nullptr);
    ~FlowScene() = default;

private:
    std::shared_ptr<DataModelRegistry> m_model_registory_;
};

#endif // FLOWSCENE_H
