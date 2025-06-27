#include "forgot.h"
#include "ui_forgot.h"

Forgot::Forgot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Forgot)
{
    ui->setupUi(this);
}

Forgot::~Forgot()
{
    delete ui;
}

void Forgot::on_lineEdit_textChanged(const QString &arg1)
{

}

