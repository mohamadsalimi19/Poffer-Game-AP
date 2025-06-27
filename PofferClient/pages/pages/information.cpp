#include "information.h"
#include "ui_information.h"
#include<menu.h>
#include<QPainter>
information::information(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::information)
{



    this->setFixedSize(this->width(), this->height());
    ui->setupUi(this);
}

void information::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void information::set(QString user,QString nam,QString lastnam, QString gmai,QString phone_nu){
    username = user;
    nam = name;
    lastname = lastnam;
    gmail = gmai;
    phone_num = phone_nu;
}











information::~information()
{
    delete ui;
}
