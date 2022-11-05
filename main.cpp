#include <QApplication>
#include <QMainWindow>

#include "flowscene.h"
#include "flowview.h"
#include "imageloadermodel.h"
#include "imageshowmodel.h"

static std::shared_ptr<DataModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<DataModelRegistry>();

    // BmTODO 添加需要注册的model
    ret->registerModel<ImageLoaderModel>();
    ret->registerModel<ImageShowModel>();

    return ret;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FlowScene scene(registerDataModels());
    FlowView view(&scene);
    view.resize(800, 600);
    view.show();

    return app.exec();
}
