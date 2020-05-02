#include "dialogmmplayback.h"
#include "ui_dialogmmplayback.h"

DialogMMPlayBack::DialogMMPlayBack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMMPlayBack)
{
    ui->setupUi(this);
}

DialogMMPlayBack::~DialogMMPlayBack()
{
    delete ui;
}
