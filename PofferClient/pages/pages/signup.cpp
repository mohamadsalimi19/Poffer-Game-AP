#include "signup.h"
#include "ui_signup.h"
#include"qmessagebox.h"
#include <QRegularExpression>
#include <QCryptographicHash>
#include<socketmanager.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);
}

signup::~signup()
{
    delete ui;
}

QString hashPasswordSimple(const QString& password) {
    QByteArray passBytes = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(passBytes, QCryptographicHash::Sha256);
    return hash.toHex();
}


void signup::on_lineEdit_textEdited(const QString &arg1) // name;
{
    name = arg1;

}


void signup::on_lineEdit_2_textChanged(const QString &arg1) // lastname;
{
    lastname = arg1;

}


void signup::on_lineEdit_3_textChanged(const QString &arg1) // username
{

    username = arg1;

}


void signup::on_lineEdit_4_textChanged(const QString &arg1) //pass
{
    pass = hashPasswordSimple(arg1);
}


void signup::on_lineEdit_5_textChanged(const QString &arg1) //email
{

    gmail = arg1;
}


void signup::on_lineEdit_6_textChanged(const QString &arg1)//phone_num
{

    phone_num = arg1;

}



void  signup::set_filds(){
    name = "";
    username="";
    pass="";
    gmail="";
    lastname = "";
    phone_num = "";
}
bool isValidEmail(const QString& email) {
    QRegularExpression regex(R"((^[\w\.-]+)@([\w\.-]+)\.([\w\.]{2,})$)"); // check for email is correct

    return regex.match(email).hasMatch();
}




bool isValidIranPhoneNumber(const QString& phone) { // check phone num is correct
    QRegularExpression regex(R"(^09\d{9}$)");
    return regex.match(phone).hasMatch();
}

void signup::makejason(){
    QJsonObject payloadObject;
    payloadObject["name"] = name;
    payloadObject["username"] = username;
    payloadObject["pass"] = pass;
    payloadObject["lastname"] = lastname;
    payloadObject["gmail"] = gmail;
    payloadObject["phone_number"] = phone_num;
    QJsonObject mainobject;
    mainobject["Command"] = "signup";
    mainobject["payloadObject"] = payloadObject;
    QJsonDocument doc(mainobject);
    jason_to_send = doc.toJson(QJsonDocument::Compact);
}




bool signup::read_jason(QByteArray res){
    auto docjason = QJsonDocument::fromJson(res);
    QJsonObject mainobject = docjason.object();
    if(mainobject["Command"]=="auth_success"){
        return true;
    }
    else{
        QMessageBox::warning(this,"warning",mainobject["message"].toString());
    }
    return false;

    }








void signup::on_pushButton_clicked()
{
    if(name==""||username==""||pass==""||gmail==""||lastname == ""||phone_num == ""){

        QMessageBox::warning(this,"warning","please enter all filds");
    }

    else if(!isValidEmail(gmail)){

        QMessageBox::warning(this,"warning","email is not correct");

    }

    else if(!isValidIranPhoneNumber(phone_num)){

        QMessageBox::warning(this,"warning","phone num is not correct");
    }

    else{
        makejason();
       SocketManager* mysocket;
       mysocket->connectToServer("192.168.88.229",80);
       mysocket->sendData(jason_to_send);
       QByteArray response = mysocket->get_response();
       if(read_jason(response)){

           // menu

       }
    }


}
