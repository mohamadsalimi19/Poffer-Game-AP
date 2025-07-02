#include "forgot.h"
#include "ui_forgot.h"
#include<signup.h>
#include"QPainter"
#include"mainwindow.h"
#include<QMessageBox>
Forgot::Forgot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Forgot)
{
    ui->setupUi(this);
    mysocket  = new SocketManager();
    connect(mysocket, &SocketManager::dataReceived, this, &Forgot::onServerResponse);
    connect(mysocket,&SocketManager::connected,this,[=](){
        make_json();
        mysocket->sendData(json_to_send);
    });
    connect(mysocket->get_socket(),&QTcpSocket::errorOccurred,this,[=](QAbstractSocket::SocketError socketError){
        if (socketError == QAbstractSocket::HostNotFoundError) {
            QMessageBox::warning(this, "Error", "سرور یافت نشد. لطفا IP را چک کنید.");
        }
    });

    connect(mysocket,&SocketManager::dataReceived,this,[=](QByteArray d){
        QMessageBox::warning(this, "warning", d);
    });


}



Forgot::~Forgot()
{
    delete ui;
}

void Forgot::on_lineEdit_textChanged(const QString &arg1)
{
    phonenum = arg1;
}


void Forgot::on_lineEdit_2_textEdited(const QString &arg1)
{
    newpass = hashPasswordSimple(arg1);
}




void Forgot::make_json(){
    QJsonObject playload;
    playload["username"] = username;
    playload["phone_number"] = phonenum;
    playload["newpassword"] = newpass;
    QJsonObject mainobject;
    mainobject["command"] = "forgot_password";
    mainobject["payload"] = playload;
    QJsonDocument doc(mainobject);
     json_to_send = doc.toJson(QJsonDocument::Compact);
}


bool Forgot::read_json(QByteArray res){
    QJsonDocument doc = QJsonDocument::fromJson(res);
    QJsonObject mainobj = doc.object();
    auto playload = mainobj["payload"].toObject();
    if(1){
        return true;
    }
    else{
        return false;
    }

}


void Forgot::on_pushButton_clicked()
{
    mysocket->connectToServer(IP,8888);



}


void Forgot::onServerResponse(QByteArray data){
    if (read_json(data)) {
        this->close();
        Login* l = new Login();
        this->close();
        l->show();
    }

    else{
    }


}

void Forgot::on_pushButton_2_clicked()
{
    this->close();
    Login* lp = new Login();
    lp->show();
}


void Forgot::on_lineEdit_3_textChanged(const QString &arg1)
{

    username = arg1;

}

