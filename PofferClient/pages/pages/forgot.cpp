#include "forgot.h"
#include "ui_forgot.h"
#include<signup.h>
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
    phonenum = arg1;
}


void Forgot::on_lineEdit_2_textEdited(const QString &arg1)
{
    newpass = hashPasswordSimple(arg1);
}




void Forgot::make_json(){
    QJsonObject playload;
    playload["phone_number"] = phonenum;
    playload["newpassword"] = newpass;
    QJsonObject mainobject;
    mainobject["command"] = "forgot";
    mainobject["payload"] = playload;
    QJsonDocument doc(mainobject);
    QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);
}


bool Forgot::read_json(QByteArray res){
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


void Forgot::on_pushButton_clicked()
{
    SocketManager* mysocket;
    //mysocket->connectToServer()
    //make_json();
    //mysocket->sendData(json_to_send);
    if(read_json(mysocket->get_response())){
        QMessageBox::warning(this,"warning", " your password was change");
        this->close();
        Login* lp = new Login();
        lp->show();
    }

}



void Forgot::on_pushButton_2_clicked()
{
    this->close();
    Login* lp = new Login();
    lp->show();
}

