#ifndef FORGOT_H
#define FORGOT_H

#include <QWidget>
#include"socketmanager.h"
namespace Ui {
class Forgot;
}

class Forgot : public QWidget
{
    Q_OBJECT
    QString phonenum;
    QString newpass="";
    void make_json();
    bool read_json(QByteArray ar);
    QByteArray json_to_send;
    SocketManager* mysocket;
    QString username;

public:
    explicit Forgot(QWidget *parent = nullptr);
    ~Forgot();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void onServerResponse(QByteArray data);


    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::Forgot *ui;
};

#endif // FORGOT_H
