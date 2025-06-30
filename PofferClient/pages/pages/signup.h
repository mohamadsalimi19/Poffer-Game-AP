#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include<login.h>
namespace Ui {
class signup;
}

class signup : public QWidget
{
    Q_OBJECT
    SocketManager* mysocket;
    QString name;
    QString lastname;
    QString username;
    QString pass;
    QString gmail;
    QString phone_num;
    QByteArray jason_to_send;

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();
    void makejason();
    void set_filds();
    bool read_jason(QByteArray res);
    void onServerResponse(QByteArray data);


private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_pushButton_clicked();



private:
    Ui::signup *ui;
};
QString hashPasswordSimple(const QString& password);
bool isValidIranPhoneNumber(const QString& phone);
bool isValidEmail(const QString& email);
#endif // SIGNUP_H
