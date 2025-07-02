#include "menu.h"
#include "ui_menu.h"
#include<QPainter>
#include <QResizeEvent>
#include<QPushButton>
#include<information.h>
#include"start.h"
menu::menu(  QString user , SocketManager* s , QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menu)

{

    username = user;
    mysocket = s;
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    ui->label->setStyleSheet(R"(
    QLabel {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #6a11cb, stop:1 #2575fc
        );
        color: white;
        font: 16pt "Segoe UI";
        padding: 10px 20px;
        border-radius: 12px;
        border: 2px solid #3b3b98;
        text-align: center;
        /* سایه ملایم متن */
        text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.4);
    }
)");


    ui->pushButton->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #56ab2f, stop:1 #a8e063
        );
        color: white;
        font: bold 14px "Segoe UI";
        padding: 10px 20px;
        border-radius: 15px;
        border: 2px solid #4caf50;
        /* سایه ملایم دکمه */
        box-shadow: 3px 3px 5px rgba(0, 0, 0, 0.3);
    }

    QPushButton:hover {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #a8e063, stop:1 #56ab2f
        );
    }

    QPushButton:pressed {
        background-color: #4caf50;
        border-style: inset;
    }
)");

    ui->pushButton_2->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #56ab2f, stop:1 #a8e063
        );
        color: white;
        font: bold 14px "Segoe UI";
        padding: 10px 20px;
        border-radius: 15px;
        border: 2px solid #4caf50;
        /* سایه ملایم دکمه */
        box-shadow: 3px 3px 5px rgba(0, 0, 0, 0.3);
    }

    QPushButton:hover {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #a8e063, stop:1 #56ab2f
        );
    }

    QPushButton:pressed {
        background-color: #4caf50;
        border-style: inset;
    }
)");

    ui->pushButton_3->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #56ab2f, stop:1 #a8e063
        );
        color: white;
        font: bold 14px "Segoe UI";
        padding: 10px 20px;
        border-radius: 15px;
        border: 2px solid #4caf50;
        /* سایه ملایم دکمه */
        box-shadow: 3px 3px 5px rgba(0, 0, 0, 0.3);
    }

    QPushButton:hover {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #a8e063, stop:1 #56ab2f
        );
    }

    QPushButton:pressed {
        background-color: #4caf50;
        border-style: inset;
    }
)");



    //this->setStyleSheet("background-image: url(:/images.jpg);");
}


void menu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}






menu::~menu()
{
    delete ui;
}




void menu::set(QString user,QString nam,QString lastnam, QString gmai,QString phone_nu,QString pass, SocketManager* s){
    username = user;
    nam = name;
    lastname = lastnam;
    gmail = gmai;
    phone_num = phone_nu;
    password = pass;
    mysocket =s;
}


void menu::on_pushButton_3_clicked()
{
    information* inft = new information(username , mysocket);
    this->close();
    inft->show();
}


void menu::on_pushButton_2_clicked(){

    start* sp = new start(username,mysocket);
    this->close();
    sp->show();


}

