#ifndef FLOWSCENE_H
#define FLOWSCENE_H

#include <QGraphicsScene>

/**
 * @brief 流场景
 */
class FlowScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit FlowScene(QObject *parent = nullptr);
};

#endif // FLOWSCENE_H
