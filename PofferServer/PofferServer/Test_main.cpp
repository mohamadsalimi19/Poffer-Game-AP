#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonObject>

// تمام کلاس‌هایی که نیاز داریم را include می‌کنیم
#include "usermanager.h"
#include "gamemanager.h"
#include "player.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "--- Starting Full Server Logic Test ---";

    // برای یک تست تمیز، فایل کاربران قبلی را پاک می‌کنیم
    QFile::remove("users.json");

    // گرفتن نمونه از مدیران سینگلتون ما
    UserManager* userManager = UserManager::instance();
    GameManager* gameManager = GameManager::instance();

    // ۱. ثبت‌نام دو بازیکن
    QJsonObject p1_data;
    p1_data["username"] = "mohammad_final";
    p1_data["password_hash"] = "p1_pass";
    userManager->signup(p1_data);

    QJsonObject p2_data;
    p2_data["username"] = "babak_final";
    p2_data["password_hash"] = "p2_pass";
    userManager->signup(p2_data);

    qDebug() << "\n[Test Step 1] Signup: Two players created.";

    // ۲. لاگین کردن هر دو بازیکن
    Player* player1 = nullptr;
    Player* player2 = nullptr;
    userManager->login("mohammad_final", "p1_pass", player1);
    userManager->login("babak_final", "p2_pass", player2);

    if (player1 && player2) {
        qDebug() << "[Test Step 2] Login: Both players logged in successfully.";
    } else {
        qCritical() << "[Test Step 2] Login: FAILED!";
        return -1;
    }

    // در دنیای واقعی، هر بازیکن ClientHandler خودش را دارد. اینجا شبیه‌سازی می‌کنیم.
    // player1->setHandler(...);
    // player2->setHandler(...);

    // ۳. ارسال درخواست بازی از طرف هر دو بازیکن
    qDebug() << "\n[Test Step 3] Game Request: Both players are requesting a game...";
    gameManager->playerWantsToPlay(player1);
    gameManager->playerWantsToPlay(player2);

    qDebug() << "\n--- Full Server Logic Test Finished ---";
    qDebug() << "Check the logs above to see if a GameSession was created and a new round started.";

    return 0;
}
