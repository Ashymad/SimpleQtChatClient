#-------------------------------------------------
#
# Project created by QtCreator 2015-05-08T20:11:23
#
#-------------------------------------------------

QT       += core gui network 

win32 { QT += winextras }

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app
RC_FILE = ChatClient.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    clientfactory.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    clientfactory.h \
    client.h

FORMS    += mainwindow.ui \
    client.ui

DISTFILES +=
