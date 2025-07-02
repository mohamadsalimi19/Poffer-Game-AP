#include "signup.h"
#include "ui_signup.h"
#include <QRegularExpression>
#include <QCryptographicHash>
#include<socketmanager.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include"QTimer"
#include"QPainter"
#include<QMessageBox>
#include"mainwindow.h"
signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    mysocket = new SocketManager();
    connect(mysocket, &SocketManager::dataReceived, this, &signup::onServerResponse);
    ui->setupUi(this);


    this->setFixedSize(this->width(), this->height());
    ui->label->setStyleSheet(R"(
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
    ui->label_8->setStyleSheet(R"(
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
    ui->label_10->setStyleSheet(R"(
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
    ui->label_11->setStyleSheet(R"(
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
    ui->label_12->setStyleSheet(R"(
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
    ui->label_13->setStyleSheet(R"(
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
    ui->label_14->setStyleSheet(R"(
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




}





void signup::onServerResponse(QByteArray data) {
    if (read_jason(data)) {
        this->close();
        Login* l = new Login();
        this->close();
        l->show();
    }
    else{


    }
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
    pass = arg1;
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
    payloadObject["password_hash"] = hashPasswordSimple(pass);
    payloadObject["lastname"] = lastname;
    payloadObject["gmail"] = gmail;
    payloadObject["phone_number"] = phone_num;
    QJsonObject mainobject;
    mainobject["command"] = "signup";
    mainobject["payload"] = payloadObject;
    QJsonDocument doc(mainobject);
    jason_to_send = doc.toJson(QJsonDocument::Compact);
}

bool signup::read_jason(QByteArray res) {
    auto docjason = QJsonDocument::fromJson(res);
    QJsonObject mainobject = docjason.object();
    QJsonObject payload = mainobject["payload"].toObject();
    if (mainobject["response"] == "auth_success") {
        return true;
    }
    else {
        QString errorMessage = payload["message"].toString();
        if (errorMessage.isEmpty()) {
            errorMessage = "خطای نامشخص از سرور دریافت شد.";
        }
        QMessageBox::warning(this, "خطا در ثبت‌نام", errorMessage);
        return false;
    }
}


void signup::on_pushButton_clicked()
{
    if (name == "" || username == "" || gmail == "" || lastname == "" || phone_num == "") {
        QMessageBox::warning(this, "اطلاعات ناقص", "لطفاً همه فیلدها را به‌طور کامل پر کنید.");
    }
    else if (pass.size() < 7) {
        QMessageBox::warning(this, "رمز عبور کوتاه", "رمز عبور باید حداقل ۷ کاراکتر باشد.");
    }
    else if (!isValidEmail(gmail)) {
        QMessageBox::warning(this, "ایمیل نامعتبر", "لطفاً یک ایمیل معتبر وارد کنید.");
    }
    else if (!isValidIranPhoneNumber(phone_num)) {
        QMessageBox::warning(this, "شماره نامعتبر", "لطفاً یک شماره موبایل معتبر ایران وارد کنید.");
    }




    else{
       makejason();
       mysocket->connectToServer("127.0.0.1",8888);
       mysocket->sendData(jason_to_send);
    }

}



    void signup::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        QPixmap pixmap(":/images.jpg");
        painter.drawPixmap(this->rect(), pixmap);
        QWidget::paintEvent(event);
    }



    void signup::on_pushButton_2_clicked()
    {
        Login*  l = new Login();
        this->close();
        l->show();
    }

