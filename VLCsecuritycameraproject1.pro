#-------------------------------------------------
#
# Project created by QtCreator 2013-08-15T12:27:30
#
#-------------------------------------------------

QT       += core gui

TARGET = VLCsecuritycameraproject1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    player.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lvlc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lvlc
else:symbian: LIBS += -lvlc
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lvlc

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include
