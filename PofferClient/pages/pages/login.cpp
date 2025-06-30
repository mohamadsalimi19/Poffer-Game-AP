#include "login.h"
#include "ui_login.h"
#include<signup.h>
#include<forgot.h>
#include<menu.h>
#include"QTimer"
Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    mysocket = new SocketManager();

    connect(mysocket, &SocketManager::dataReceived, this, &Login::onServerResponse);
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
       pass =hashPasswordSimple(arg1);
}

void Login::onServerResponse(QByteArray data) {
    if (read_json(data)) {
        QMessageBox::information(this, "Success", "Signed up successfully!");
        this->close();
        menu* l = new menu();
        this->close();
        l->show();
    }
    else{
        QMessageBox::information(this, "Success", data);
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
        QMessageBox::warning(this,"warning", payload["message"].toString());
        return false;
    }

}

void Login::on_pushButton_clicked()
{

    mysocket->connectToServer("127.0.0.1",8888);
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

