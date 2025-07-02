#include "login.h"
#include "ui_login.h"
#include<signup.h>
#include<forgot.h>
#include<menu.h>
#include"QTimer"
#include"QPainter"
#include"mainwindow.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    mysocket = new SocketManager();
    connect(mysocket, &SocketManager::dataReceived, this, &Login::onServerResponse);
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
    ui->label_4->setStyleSheet(R"(
    QLabel {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #6a11cb, stop:1 #2575fc
        );
        color: white;
        font: 10pt "Segoe UI";
        padding: 10px 20px;
        border-radius: 12px;
        border: 2px solid #3b3b98;
        text-align: center;
        /* سایه ملایم متن */
        text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.4);
    }
)");
    ui->label_7->setStyleSheet(R"(
    QLabel {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #6a11cb, stop:1 #2575fc
        );
        color: white;
        font: 10pt "Segoe UI";
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








}
void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

Login::~Login()
{
    delete ui;
}




void Login::on_lineEdit_textEdited(const QString &arg1)
{

    username = arg1;

}


void Login::on_lineEdit_2_textEdited(const QString &arg1)
{
       pass = hashPasswordSimple(arg1);
}



void Login::onServerResponse(QByteArray data) {
    if (read_json(data)) {
       // QMessageBox::information(this, "Success", "Login up successfully!");
        this->close();
        menu* l = new menu(username , mysocket);
        l->set(username,name,lastname,gmail,phone_num,pass,mysocket);
        this->close();
        l->show();
    }
    else{
       // QMessageBox::information(this, "Success", data);
    }
}

void Login::make_json(){
    QJsonObject playload;
    playload["username"] = username;
    playload["password_hash"] = pass;
    QJsonObject mainobject;
    mainobject["command"] = "login";
    mainobject["payload"] = playload;
    QJsonDocument doc(mainobject);
    json_to_send = doc.toJson(QJsonDocument::Compact);
}
bool Login::read_json(QByteArray res){
    QJsonDocument doc = QJsonDocument::fromJson(res);
    QJsonObject mainobj = doc.object();
    auto payload = mainobj["payload"].toObject();
    if(mainobj["response"].toString()=="auth_success"){
        name = payload["name"].toString();
        lastname = payload["lastname"].toString();
        gmail = payload["email"].toString();
        phone_num = payload["phone_number"].toString();
        return true;
    }
    else{
        return false;
    }

}

void Login::on_pushButton_clicked()
{

    mysocket->connectToServer(IP,8888);
    make_json();
    mysocket->sendData(json_to_send);



}


void Login::on_pushButton_2_clicked()
{
    Forgot* fp = new Forgot();
    this->close();
    fp->show();
}


void Login::on_pushButton_3_clicked()
{
    signup* s = new signup();
    s->show();
    this->close();

}





void Login::on_lineEdit_3_textEdited(const QString &arg1)
{
    username = arg1;

}

