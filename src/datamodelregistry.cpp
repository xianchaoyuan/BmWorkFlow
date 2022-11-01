#include "datamodelregistry.h"

std::unique_ptr<NodeDataModel> DataModelRegistry::create(const QString &modelName)
{
    auto it = m_registry_model_creators_.find(modelName);
    if (it != m_registry_model_creators_.end()) {
        return it->second();
    }

    return nullptr;
}
