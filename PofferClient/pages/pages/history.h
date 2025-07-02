#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include"menu.h"
#include<QJsonArray>
namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT
    QString username;
    SocketManager* socket;
    QJsonArray game_history;
    QMultiMap<QString , QString> history_map;
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit History(QString u , SocketManager* s , QWidget *parent = nullptr);
    ~History();
    void get_History();

private slots:
    void on_pushButton_clicked();

private:
    Ui::History *ui;
};

#endif // HISTORY_H
