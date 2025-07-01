#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include "usermanager.h"
#include "player.h"

void printTestResult(const QString& testName, bool passed)
{
    if (passed) {
        qDebug() << "[PASSED]" << testName;
    } else {
        qDebug() << "[FAILED]" << testName;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "--- Starting UserManager Profile-Update Test ---";

    QFile::remove("users.json");
    UserManager* manager = UserManager::instance();

    // ۱. یک کاربر تستی می‌سازیم
    QJsonObject signup_data;
    signup_data["username"] = "mohammad";
    signup_data["password_hash"] = "pass123";
    signup_data["name"] = "Mohammad";
    signup_data["lastname"] = "Salimi";
    signup_data["gmail"] = "m@test.com";
    signup_data["phone_number"] = "0910";
    manager->signup(signup_data);

    // ۲. تست آپدیت ساده (تغییر نام و نام خانوادگی)
    QJsonObject update1_data;
    update1_data["name"] = "Mohammad_Updated";
    update1_data["lastname"] = "Salimi_Updated";
    bool success1 = manager->updatePlayerProfile("mohammad", update1_data);
    Player* player_after_update1 = manager->getPlayer("mohammad");
    printTestResult("Simple profile update", success1 && player_after_update1->getFirstName() == "Mohammad_Updated");

    // ۳. تست تغییر نام کاربری
    QJsonObject update2_data;
    update2_data["username"] = "mohammad_new";
    bool success2 = manager->updatePlayerProfile("mohammad", update2_data);
    Player* player_with_old_name = manager->getPlayer("mohammad");
    Player* player_with_new_name = manager->getPlayer("mohammad_new");
    printTestResult("Username change", success2 && (player_with_old_name == nullptr) && (player_with_new_name != nullptr));

    // ۴. تست جلوگیری از نام کاربری تکراری
    // اول یک کاربر دوم می‌سازیم
    QJsonObject signup2_data;
    signup2_data["username"] = "babak";
    signup2_data["password_hash"] = "pass456";
    manager->signup(signup2_data);
    // حالا سعی می‌کنیم نام کاربر اول را به "babak" تغییر دهیم (باید شکست بخورد)
    QJsonObject update3_data;
    update3_data["username"] = "babak";
    bool success3 = manager->updatePlayerProfile("mohammad_new", update3_data);
    printTestResult("Prevent duplicate username on update", !success3);


    qDebug() << "\nAll tests finished.";
    return 0;
}
