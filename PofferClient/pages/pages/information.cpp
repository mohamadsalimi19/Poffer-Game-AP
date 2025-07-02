#include "information.h"
#include "ui_information.h"
#include <menu.h>
#include <QPainter>
#include <socketmanager.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <signup.h>
#include<QMessageBox>
#include<QTimer>
information::information(QString user, SocketManager* s, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::information),
    username(user),
    mys(s)
{
    ui->setupUi(this);

    connect(mys, &SocketManager::dataReceived, this, &information::ServerResponse);
    connect(this, &information::dataRECIVED, this, &information::update_ui);

    getINFO();
    this->setFixedSize(this->width(), this->height());
}
bool messageBoxShown = false;


void information::enter_filds() {
    ui->textBrowser->setText(username);
}

void information::update_ui() {
    ui->textBrowser->setText(username);
    ui->textBrowser_2->setText(name);
    ui->textBrowser_3->setText(lastname);
    ui->textBrowser_4->setText("*");  // برای امنیت پسورد
    ui->textBrowser_5->setText(gmail);
    ui->textBrowser_6->setText(phone_num);
}

void information::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void information::getINFO() {
    QJsonObject mainobject;
    mainobject["command"] = "request_profile_data";
    mainobject["payload"] = QJsonObject();
    QJsonDocument doc(mainobject);
    json_to_send = doc.toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);
}
/*
void information::ServerResponse(QByteArray data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    if (obj["response"].toString() == "profile_data_response") {
        QJsonObject payload = obj["payload"].toObject();
        username = payload["username"].toString();
        pass = payload["password_hash"].toString();
        name = payload["name"].toString();
        lastname = payload["lastname"].toString();
        gmail = payload["gmail"].toString();
        phone_num = payload["phone_number"].toString();
        emit dataRECIVED();
    }


    else if(obj["response"].toString() == "profile_update_success"){
        QMessageBox::warning(nullptr,"information",obj["response"].toString() );
        update_ui();
    }


    else{
    QMessageBox::warning(nullptr,"information",obj["response"].toString() );
    }

}
*/

void information::ServerResponse(QByteArray data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    QString responseType = obj["response"].toString();

    if (responseType == "profile_data_response") {
        QJsonObject payload = obj["payload"].toObject();
        username = payload["username"].toString();
        pass = payload["password_hash"].toString();
        name = payload["name"].toString();
        lastname = payload["lastname"].toString();
        gmail = payload["gmail"].toString();
        phone_num = payload["phone_number"].toString();
        emit dataRECIVED();
    }
    else if (responseType == "profile_update_success") {
        if (!messageBoxShown) {
            QMessageBox::information(this, "Information", "Profile updated successfully.");
            messageBoxShown = true;
            QTimer::singleShot(1500, this, [this](){ messageBoxShown = false; }); // ریست بعد از 1.5 ثانیه
        }
        update_ui();
    }
    else {
        if (!messageBoxShown) {
            QMessageBox::warning(this, "Information", "Response: " + responseType);
            messageBoxShown = true;
            QTimer::singleShot(1500, this, [this](){ messageBoxShown = false; }); // ریست بعد از 1.5 ثانیه
        }
    }
}


bool information::read_json(QByteArray res) {
    auto doc = QJsonDocument::fromJson(res);
    QJsonObject mainobject = doc.object();
    return mainobject["response"].toString() == "auth_success";
}

bool information::update() {
    mys->sendData(json_to_send);
    auto res = mys->get_response();
    return read_json(res);
}

// --------------------- تغییرات فیلدها ---------------------

void information::on_lineEdit_3_textEdited(const QString &arg1) {
    newusername = arg1;
}

void information::on_pushButton_clicked() {
    if (newusername.isEmpty()) return;

    QJsonObject payload;
    payload["username"] = newusername;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);

}

// --------------------- نام ---------------------

void information::on_lineEdit_5_textEdited(const QString &arg1) {
    newname = arg1;
}

void information::on_pushButton_6_clicked() {
    if (newname.isEmpty()){
        QMessageBox::warning(nullptr,"information","please enter your stata" );

        return;
    }
    QJsonObject payload;
    payload["name"] = newname;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);


}

// --------------------- نام خانوادگی ---------------------

void information::on_lineEdit_4_textEdited(const QString &arg1) {
    newlastname = arg1;
}

void information::on_pushButton_3_clicked() {
    if (newlastname.isEmpty()){
        QMessageBox::warning(nullptr,"information","please enter your stata" );

        return;
    }
    QJsonObject payload;
    payload["lastname"] = newlastname;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);


}

// --------------------- پسورد ---------------------

void information::on_lineEdit_2_textEdited(const QString &arg1) {
    newpass = hashPasswordSimple(arg1);
}

void information::on_pushButton_4_clicked() {
    if (newpass.isEmpty()){
        QMessageBox::warning(nullptr,"information","please enter your fild" );
        return;
    }
    QJsonObject payload;
    payload["password_hash"] = newpass;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);


}

// --------------------- ایمیل ---------------------

void information::on_lineEdit_6_textEdited(const QString &arg1) {
    newgmail = arg1;
}

void information::on_pushButton_5_clicked() {
    if (!isValidEmail(newgmail)){
        QMessageBox::warning(nullptr,"information","please enter corect email" );

        return;
    }
    QJsonObject payload;
    payload["gmail"] = newgmail;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);


}

// --------------------- شماره تلفن ---------------------

void information::on_lineEdit_textEdited(const QString &arg1) {
    newphone_num = arg1;
}

void information::on_pushButton_2_clicked() {
    if (!isValidIranPhoneNumber(newphone_num)){
        QMessageBox::warning(nullptr,"information","please enter correct phone num" );

        return;
    }
    QJsonObject payload;
    payload["phone_number"] = newphone_num;

    QJsonObject mainobject;
    mainobject["command"] = "edit_profile";
    mainobject["payload"] = payload;

    json_to_send = QJsonDocument(mainobject).toJson(QJsonDocument::Compact);
    mys->sendData(json_to_send);

}

// --------------------- بازگشت به منو ---------------------

void information::on_pushButton_7_clicked() {
    this->close();
    menu* mn = new menu(username, mys);
    mn->set(username, name, lastname, gmail, phone_num, pass, mys);
    mn->show();
}

information::~information() {
    delete ui;
}
