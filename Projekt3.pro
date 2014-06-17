QT        += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET     = Projekt3
TEMPLATE   = app


SOURCES   += main.cpp\
             MainWindow.cpp \
             Game.cpp \
             NewGameDialog.cpp \
             Player.cpp \
             Canvas.cpp

HEADERS   +=  MainWindow.h \
             Game.h \
             NewGameDialog.h \
             Player.h \
             Canvas.h

FORMS     += MainWindow.ui \
             NewGameDialog.ui

RESOURCES += res.qrc

RC_FILE    = res.rc
