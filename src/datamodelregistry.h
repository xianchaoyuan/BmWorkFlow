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
    using CategorysSet = std::set<QString>;

    DataModelRegistry();

private:

};

#endif // DATAMODELREGISTRY_H
