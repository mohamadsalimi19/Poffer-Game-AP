#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>

namespace Ui {
class information;
}

class information : public QWidget
{
    Q_OBJECT
    QString username = "user";
    QString name = "name";
    QString lastname = "last";
    QString gmail = "g";
    QString phone_num = "0910";
    QString pass = "*";
    QByteArray json_to_send;
    QString newusername="";
    QString newpass="";
    QString newname="";
    QString newlastname="";
    QString newgmail="";
    QString newphone_num="";

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    void set(QString username,QString name,QString lastname, QString gmail,QString phone_num,QString pass);
    void enter_filds();
    void make_json();
    bool read_json(QByteArray ar);
    bool update();
    explicit information(QWidget *parent = nullptr);
    ~information();

private slots:


    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();


    void on_lineEdit_5_textEdited(const QString &arg1);

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_lineEdit_6_textEdited(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::information *ui;
};

#endif // INFORMATION_H
