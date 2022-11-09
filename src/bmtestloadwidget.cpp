#include "bmtestloadwidget.h"
#include "ui_bmtestloadwidget.h"

BmTestLoadWidget::BmTestLoadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BmTestLoadWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

BmTestLoadWidget::~BmTestLoadWidget()
{
    delete ui;
}
