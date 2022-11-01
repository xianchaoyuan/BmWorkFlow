#include <QApplication>
#include <QMainWindow>

#include "flowscene.h"
#include "flowview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FlowScene scene;
    FlowView view(&scene);
    view.resize(800, 600);
    view.show();

    return app.exec();
}
