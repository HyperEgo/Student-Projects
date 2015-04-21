#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T01:21:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core
QT += network
QT += gui
QT += widgets

TARGET = ReceiveNode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui
