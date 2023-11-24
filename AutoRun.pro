QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11

LIBS    += -luser32 -lAdvapi32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    add.h \
    mainwindow.h \
    global.h \
    regItem.h

FORMS += \
    add.ui \
    mainwindow.ui

TRANSLATIONS += \
    AutoRun_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    logo.rc

RC_FILE += \
    logo.rc
