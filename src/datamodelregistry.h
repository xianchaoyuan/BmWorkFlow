#ifndef DATAMODELREGISTRY_H
#define DATAMODELREGISTRY_H

#include <iostream>
#include <memory>
#include <set>
#include <functional>
#include <unordered_map>

#include <QString>

#include "nodedatamodel.h"

/**
 * @brief 模型注册
 */
class DataModelRegistry
{
public:
    using RegistryItemPtr     = std::unique_ptr<NodeDataModel>;
    using RegistryItemCreator = std::function<RegistryItemPtr()>;
    using RegistryModelCreatorsMap  = std::unordered_map<QString, RegistryItemCreator>;
    using RegistryModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = std::set<QString>;

    DataModelRegistry() = default;
    ~DataModelRegistry() = default;

public:
    template<typename ModelType>
    void registerModel(RegistryItemCreator creator, const QString &category = "Nodes")
    {
        const QString name = creator()->name();
        if (!m_registry_model_creators_.count(name)) {
            m_categories_.insert(category);
            m_registry_model_creators_[name] = std::move(creator);
            m_registry_models_category_[name] = category;
        }
    }

    template<typename ModelType>
    void registerModel(const QString &category = "Nodes")
    {
        RegistryItemCreator creator = []() {
            return std::make_unique<ModelType>();
        };
        registerModel<ModelType>(std::move(creator), category);
    }

    //! 创建model ptr
    std::unique_ptr<NodeDataModel> create(const QString &modelName);

    const RegistryModelCreatorsMap &registeredModelCreators() const {
        return m_registry_model_creators_;
    }

    const RegistryModelsCategoryMap &registeredModelsCategory() const {
        return m_registry_models_category_;
    }

private:
    RegistryModelCreatorsMap m_registry_model_creators_;    // creators映射
    RegistryModelsCategoryMap m_registry_models_category_;  // categorys映射

    CategoriesSet m_categories_;  // category集合
};

#endif // DATAMODELREGISTRY_H
