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
    QByteArray json_to_send;

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void make_json();
    bool read_json(QByteArray ar);

private slots:

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
