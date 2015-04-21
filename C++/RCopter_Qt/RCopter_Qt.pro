#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T22:33:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT += core
QT += gui
TARGET = RCopter_Qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    images.qrc

OTHER_FILES += \
    images/arrow_down_red.png \
    images/arrow_left_red.png \
    images/arrow_right_red.png \
    images/arrow_up_red.png \
    images/arrow_down_grey.png \
    images/arrow_left_grey.png \
    images/arrow_right_grey.png \
    images/arrow_up_grey.png \
    images/green_lock.png \
    images/tron_console02.jpg

