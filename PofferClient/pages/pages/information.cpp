#include "information.h"
#include "ui_information.h"
#include<menu.h>
#include<QPainter>
#include<socketmanager.h>
#include<QJsonDocument>
#include <QJsonObject>
#include<signup.h>
#include<menu.h>
information::information(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::information)
{


    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    ui->textBrowser->setText(username);
    ui->textBrowser_2->setText(name);
    ui->textBrowser_3->setText(lastname);
    ui->textBrowser_4->setText("*");
    ui->textBrowser_5->setText(gmail);
    ui->textBrowser_6->setText(phone_num);
}

void information::enter_filds(){
    ui->textBrowser->setText(username);
}


void information::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void information::set(QString user,QString nam,QString lastnam, QString gmai,QString phone_nu,QString passs ,SocketManager* s){
    username = user;
    nam = name;
    lastname = lastnam;
    gmail = gmai;
    phone_num = phone_nu;
    pass = passs;
    mys = s;
}



void information::make_json(){

    QJsonObject payloadObject;
    payloadObject["name"] = name;
    payloadObject["username"] = username;
    payloadObject["pass"] = pass;
    payloadObject["lastname"] = lastname;
    payloadObject["gmail"] = gmail;
    payloadObject["phone_number"] = phone_num;
    QJsonObject mainobject;
    mainobject["Command"] = "update_information";
    mainobject["payload"] = payloadObject;
    QJsonDocument doc(mainobject);
    json_to_send = doc.toJson(QJsonDocument::Compact);
}


bool information::read_json(QByteArray res){

    auto docjason = QJsonDocument::fromJson(res);
    QJsonObject mainobject = docjason.object();
    if(mainobject["response"]=="auth_success"){
        return true;
    }
    else{
        QMessageBox::warning(this,"warning",mainobject["message"].toString());
    }
    return false;

}
information::~information()
{
    delete ui;
}




void information::on_lineEdit_3_textEdited(const QString &arg1)
{
    newusername = arg1;



}


void information::on_pushButton_clicked()
{
    QJsonObject payloadObject;
    payloadObject["name"] = name;
    payloadObject["username"] = newusername;
    payloadObject["pass"] = pass;
    payloadObject["lastname"] = lastname;
    payloadObject["gmail"] = gmail;
    payloadObject["phone_number"] = phone_num;
    QJsonObject mainobject;
    mainobject["Command"] = "update_username";
    mainobject["payload"] = payloadObject;
    QJsonDocument doc(mainobject);
    json_to_send = doc.toJson(QJsonDocument::Compact);
    if(update()){
        username = newusername;
        ui->textBrowser->setText(username);
    }
}

bool information::update(){
    SocketManager* mys = new SocketManager();
    //mys->connectToServer();
    mys->sendData(json_to_send);
    auto res = mys->get_response();
    if(read_json(res)){
        QMessageBox::warning(this,"warning"," your information update succsecfully");
        return true;
    }
    else{
        return false;
    }
}






void information::on_lineEdit_5_textEdited(const QString &arg1)
{
    newname = arg1;
}

void information::on_pushButton_6_clicked()
{

    if(newname=="") {
        QMessageBox::warning(this,"warning","please enter the fild");
        return;
    }

    if(update()){
        name = newname;
        ui->textBrowser_2->setText(name);
    }
}

void information::on_lineEdit_4_textEdited(const QString &arg1)
{
    newlastname =arg1;
}


void information::on_pushButton_3_clicked()
{
    if(newlastname=="") {
        QMessageBox::warning(this,"warning","please enter the fild");
        return;
    }
    if(update()){
        lastname = newlastname;
        ui->textBrowser_2->setText(name);
    }
}






void information::on_lineEdit_2_textEdited(const QString &arg1)
{
    newpass = hashPasswordSimple(arg1);
}


void information::on_pushButton_4_clicked()
{
    if(newpass=="") {
        QMessageBox::warning(this,"warning","please enter the fild");
        return;
    }
    if(update()){
        pass = newpass;
    }
}


void information::on_lineEdit_6_textEdited(const QString &arg1)
{
    newgmail = arg1;
}







void information::on_pushButton_5_clicked()
{
    if(isValidEmail(newgmail)){
        if(update()){
            gmail = newgmail;
            ui->textBrowser_5->setText(gmail);
        }

    }

    else{
        QMessageBox::warning(this,"warning","your email is wrong");
    }


}


void information::on_lineEdit_textEdited(const QString &arg1)
{
    newphone_num = arg1;
}


void information::on_pushButton_2_clicked()
{
    if(isValidIranPhoneNumber(newphone_num)){
        if(update()){
            phone_num = newphone_num;
            ui->textBrowser_6->setText(phone_num);
        }
    }
    else{
        QMessageBox::warning(this,"warning","your phone num is wrong");
    }

}


void information::on_pushButton_7_clicked()
{
    this->close();
    menu* mn = new menu();
    mn->set(username,name,lastname,gmail,phone_num,pass,mys);
    this->close();
    mn->show();
}

