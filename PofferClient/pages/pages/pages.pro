QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card.cpp \
    forgot.cpp \
    information.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    poffer.cpp \
    signup.cpp \
    socketmanager.cpp \
    start.cpp

HEADERS += \
    card.h \
    forgot.h \
    information.h \
    login.h \
    mainwindow.h \
    menu.h \
    poffer.h \
    signup.h \
    socketmanager.h \
    start.h

FORMS += \
    forgot.ui \
    information.ui \
    login.ui \
    mainwindow.ui \
    menu.ui \
    poffer.ui \
    signup.ui \
    start.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
