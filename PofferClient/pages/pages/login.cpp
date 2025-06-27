#include "login.h"
#include "ui_login.h"
#include<signup.h>
Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
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
    auto playload = mainobj["payload"].toObject();
    if(mainobj["response"].toString()=="auth_success"){
        return true;
    }
    else{
        QMessageBox::warning(this,"warning", playload["message"].toString());
        return false;
    }

}

void Login::on_pushButton_clicked()
{

    SocketManager* mysocket;
    //mysocket->connectToServer()
    //make_json();
    //mysocket->sendData(json_to_send);
    if(read_json(mysocket->get_response())){

        // menu code

    }
}


void Login::on_pushButton_2_clicked()
{

}

