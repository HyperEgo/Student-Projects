#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T20:17:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core
QT += network
QT += gui
QT += widgets

TARGET = SendNode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h

FORMS    += mainwindow.ui
