#ifndef FORGOT_H
#define FORGOT_H

#include <QWidget>

namespace Ui {
class Forgot;
}

class Forgot : public QWidget
{
    Q_OBJECT

public:
    explicit Forgot(QWidget *parent = nullptr);
    ~Forgot();

private:
    Ui::Forgot *ui;
};

#endif // FORGOT_H
