#ifndef BMTESTLOADWIDGET_H
#define BMTESTLOADWIDGET_H

#include <QWidget>

namespace Ui {
class BmTestLoadWidget;
}

class BmTestLoadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BmTestLoadWidget(QWidget *parent = nullptr);
    ~BmTestLoadWidget();

private:
    Ui::BmTestLoadWidget *ui;
};

#endif // BMTESTLOADWIDGET_H
