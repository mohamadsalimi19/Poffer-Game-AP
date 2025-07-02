#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qdebug.h>
#include<QSettings>
#include<socketmanager.h>
#include<menu.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QSettings settings("MyCompany", "MyApp");
    signedIn = settings.value("signed_in", false).toBool();
}
QString IP = "";
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    if(IP!=""){



    signup* sp = new signup();
    //menu* mp = new menu();
    this->close();
    //sp->show();
    //mp->show();

    if(!signedIn){
        this->close();
        signup* sp = new signup();
        sp->show();
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("signed_in", true);
        settings.sync();
    }

    else{
        Login* lp = new Login();
        this->close();
        lp->show();


    }


    }

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{

    IP = arg1;
}

