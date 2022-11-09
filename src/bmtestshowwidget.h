#ifndef BMTESTSHOWWIDGET_H
#define BMTESTSHOWWIDGET_H

#include <QWidget>

namespace Ui {
class BmTestShowWidget;
}

class BmTestShowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BmTestShowWidget(QWidget *parent = nullptr);
    ~BmTestShowWidget();

private:
    Ui::BmTestShowWidget *ui;
};

#endif // BMTESTSHOWWIDGET_H
