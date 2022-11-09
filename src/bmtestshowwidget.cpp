#include "bmtestshowwidget.h"
#include "ui_bmtestshowwidget.h"

BmTestShowWidget::BmTestShowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BmTestShowWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

BmTestShowWidget::~BmTestShowWidget()
{
    delete ui;
}
