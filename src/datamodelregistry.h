#ifndef DATAMODELREGISTRY_H
#define DATAMODELREGISTRY_H

#include <iostream>
#include <memory>
#include <set>
#include <functional>
#include <unordered_map>

#include <QString>

#include "typeconverter.h"
#include "nodedatamodel.h"

/**
 * @brief 模型注册
 * @BmTODO 类型转化映射暂时未添加
 */
class DataModelRegistry
{
public:
    using RegistryItemPtr     = std::unique_ptr<NodeDataModel>;
    using RegistryItemCreator = std::function<RegistryItemPtr()>;
    using RegisteredModelCreatorsMap  = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = std::set<QString>;

    using RegisteredTypeConvertersMap = std::unordered_map<TypeConverterId, TypeConverter, TypeConverterIdHash>;

    DataModelRegistry() = default;
    ~DataModelRegistry() = default;

public:
    template<typename ModelType>
    void registerModel(RegistryItemCreator creator, const QString &category = "Nodes")
    {
        const QString name = creator()->name();
        if (!m_registered_model_creators_.count(name)) {
            m_categories_.insert(category);
            m_registered_model_creators_[name] = std::move(creator);
            m_registered_models_category_[name] = category;
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

    const RegisteredModelCreatorsMap &registeredModelCreators() const;
    const RegisteredModelsCategoryMap &registeredModelsCategoryAssociation() const;
    const CategoriesSet &categories() const;

    //! 类型转换
    TypeConverter getTypeConverter(NodeDataType const &d1,
                                   NodeDataType const &d2) const;

private:
    RegisteredModelCreatorsMap m_registered_model_creators_;    // creators映射
    RegisteredModelsCategoryMap m_registered_models_category_;  // categorys映射
    RegisteredTypeConvertersMap m_registered_type_converters_;  // 类型转化映射

    CategoriesSet m_categories_;  // categories集合
};

#endif // DATAMODELREGISTRY_H
