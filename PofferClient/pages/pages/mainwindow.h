#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<fstream>
#include<istream>
#include<signup.h>
#include<login.h>
QT_BEGIN_NAMESPACE

extern QString IP;


namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool signedIn;
private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
