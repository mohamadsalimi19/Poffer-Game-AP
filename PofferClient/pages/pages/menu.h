#ifndef MENU_H
#define MENU_H

#include <QWidget>

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT
    QString username;
    QString name;
    QString lastname;
    QString gmail;
    QString phone_num;
    QString password;

public:
    void set(QString username,QString name,QString lastname, QString gmail,QString phone_num , QString pass);
    explicit menu(QWidget *parent = nullptr);
    ~menu();

protected:
    void paintEvent(QPaintEvent *event) override;


private slots:
    void on_pushButton_3_clicked();

private:
    Ui::menu *ui;
};

#endif // MENU_H
