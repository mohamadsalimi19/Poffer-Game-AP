#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>

namespace Ui {
class information;
}

class information : public QWidget
{
    Q_OBJECT
    QString username;
    QString name;
    QString lastname;
    QString gmail;
    QString phone_num;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    void set(QString username,QString name,QString lastname, QString gmail,QString phone_num);
    explicit information(QWidget *parent = nullptr);
    ~information();

private:
    Ui::information *ui;
};

#endif // INFORMATION_H
