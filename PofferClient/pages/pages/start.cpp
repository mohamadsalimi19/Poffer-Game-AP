#include "start.h"
#include "ui_start.h"
#include<QJsonObject>
#include<QLabel>
#include<QVBoxLayout>
#include<QTimer>
#include"poffer.h"
start::start(const QString& un, SocketManager* client, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::start),
    username(un),
    client_socket(client)
{
    ui->setupUi(this);

    connect(client_socket, &SocketManager::dataReceived, this,&start::handle_res);

    start_game();
}


void start::set_username(QString un, SocketManager* client){
    username = un;
    client_socket = client;
}


QByteArray start::make_requset_json(){
    QJsonObject playload;
    //playload["username"] = username ;
    QJsonObject mainobject;
    mainobject["command"] = "request_game";
    mainobject["payload"] = playload;
    QJsonDocument doc(mainobject);
    return doc.toJson(QJsonDocument::Compact);
}




bool start::get_start_response(){
    auto res = client_socket->get_response();
    QJsonDocument doc = QJsonDocument::fromJson(res);
    QJsonObject mainobj = doc.object();
    auto payload = mainobj["payload"].toObject();
    if(mainobj["response"].toString()=="game_started"){
        return true;
    }
    else{
        return false;
    }
}


void start::handle_res(QByteArray a){
    QJsonDocument doc = QJsonDocument::fromJson(a);
    QJsonObject mainobj = doc.object();
    auto payload = mainobj["payload"].toObject();
    if(mainobj["response"].toString()=="game_started"){
        Poffer* g = new Poffer(client_socket, username, this->parentWidget());
        g->setGeometry(this->geometry());
        g->show();
        this->close();
    }
}


void start::start_game(){
    QLabel* statue = new QLabel(this);
    statue->setText("Searching for an opponent...");
    statue->setAlignment(Qt::AlignCenter);  // اضافه برای وسط‌چین کردن متن
    statue->setStyleSheet(R"(
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
    /* text-align: center;  --> در Qt کاربردی ندارد */
    /* سایه ملایم متن */
}
)");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(statue);
    client_socket->sendData(make_requset_json());

}



start::~start()
{
    delete ui;
}
