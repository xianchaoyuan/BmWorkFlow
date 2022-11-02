#include "datamodelregistry.h"

std::unique_ptr<NodeDataModel> DataModelRegistry::create(const QString &modelName)
{
    auto it = m_registered_model_creators_.find(modelName);
    if (it != m_registered_model_creators_.end()) {
        return it->second();
    }

    return nullptr;
}

const DataModelRegistry::RegisteredModelCreatorsMap &DataModelRegistry::registeredModelCreators() const
{
    return m_registered_model_creators_;
}

const DataModelRegistry::RegisteredModelsCategoryMap &DataModelRegistry::registeredModelsCategoryAssociation() const
{
    return m_registered_models_category_;
}

const DataModelRegistry::CategoriesSet &DataModelRegistry::categories() const
{
    return m_categories_;
}
