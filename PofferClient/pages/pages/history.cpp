#include "history.h"
#include "ui_history.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include"menu.h"
History::History(QString u, SocketManager* s, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::History),
    username(u),
    socket(s)
{
    this->setStyleSheet(R"(
    QWidget {
        background-color: #121212;
        color: #e0e0e0;
        font-family: Consolas, monospace;
        font-size: 16px;
    }

    QListWidget {
        background-color: #1e1e1e;
        color: #00ffea;
        border: 2px solid #00ffea;
        border-radius: 10px;
        padding: 5px;
    }

    QListWidget::item {
        padding: 10px;
    }

    QListWidget::item:selected {
        background-color: #00ffea;
        color: #121212;
        border-radius: 5px;
    }

    QPushButton {
        background-color: #00ffea;
        color: #121212;
        border: none;
        border-radius: 8px;
        padding: 8px 16px;
    }

    QPushButton:hover {
        background-color: #00c7b3;
    }

    QPushButton:pressed {
        background-color: #008f84;
    }
)");



    ui->setupUi(this);

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
    connect(socket, &SocketManager::dataReceived, this, [=](QByteArray data){
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) {
            QMessageBox::warning(this, "Error", "Invalid data from server.");
            return;
        }

        QJsonObject obj = doc.object();
        if (obj["response"].toString() != "profile_data_response") {
            return; // Ignore unrelated responses
        }

        QJsonArray game_history = obj["payload"].toObject()["game_history"].toArray();

        ui->listWidget->clear();
        for (auto val : game_history) {
            auto c = val.toObject();
            auto res = c["result"].toString();
            auto opp = c["opponent"].toString();
            ui->listWidget->addItem("Opponent: " + opp + " | Result: " + res);
        }
    });

    get_History();
}

void History::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap pixmap(":/images.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void History::get_History() {
    QJsonObject mainobject;
    mainobject["command"] = "request_profile_data";
    mainobject["payload"] = QJsonObject();
    QJsonDocument doc(mainobject);
    auto json_to_send = doc.toJson(QJsonDocument::Compact);
    socket->sendData(json_to_send);
}

History::~History() {
    delete ui;
}

void History::on_pushButton_clicked()
{
    menu* mn = new menu(username,socket);
    this->close();
    mn->show();

}

