#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include "usermanager.h" // کلاسی که می‌خواهیم تست کنیم

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

    qDebug() << "--- Starting UserManager Test ---";

    // قدم ۰: برای هر بار تست، فایل کاربران قبلی را پاک می‌کنیم تا از صفر شروع کنیم
    QFile::remove("users.json");
    qDebug() << "Removed old users.json for a clean test.";

    // قدم ۱: یک نمونه از مدیر کاربران می‌گیریم
    UserManager* manager = UserManager::instance();

    // --- تست ثبت‌نام ---
    qDebug() << "\n--- Testing Signup ---";

    // تست ۱.۱: ثبت‌نام موفق یک کاربر جدید
    QJsonObject user1_data;
    user1_data["username"] = "mohammad";
    user1_data["password_hash"] = "hash123";
    printTestResult("Successful signup for 'mohammad'", manager->signup(user1_data));

    // تست ۱.۲: تلاش برای ثبت‌نام مجدد با همان نام کاربری (باید شکست بخورد)
    printTestResult("Duplicate signup for 'mohammad'", !manager->signup(user1_data));


    // --- تست ورود ---
    qDebug() << "\n--- Testing Login ---";

    // تست ۲.۱: ورود موفق با اطلاعات صحیح
    printTestResult("Successful login for 'mohammad'", manager->login("mohammad", "hash123"));

    // تست ۲.۲: تلاش برای ورود با رمز عبور اشتباه (باید شکست بخورد)
    printTestResult("Failed login with wrong password", !manager->login("mohammad", "wrong_hash"));

    // تست ۲.۳: تلاش برای ورود با کاربری که وجود ندارد (باید شکست بخورد)
    printTestResult("Failed login with non-existent user", !manager->login("babak", "any_hash"));


    qDebug() << "\n--- UserManager tests finished ---";

    return 0; // از برنامه خارج شو
}
