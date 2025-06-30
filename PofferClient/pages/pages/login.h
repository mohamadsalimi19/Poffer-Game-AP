#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include"qmessagebox.h"
#include <QRegularExpression>
#include <QCryptographicHash>
#include<socketmanager.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    QString username;
    QString pass;
    QString name;
    QString lastname;
    QString gmail;
    QString phone_num;
    QByteArray json_to_send;
    SocketManager* mysocket;

public:
    explicit Login(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    ~Login();
    void make_json();
    bool read_json(QByteArray ar);
    void onServerResponse(QByteArray data);


private slots:

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_lineEdit_3_textEdited(const QString &arg1);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
