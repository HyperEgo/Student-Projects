#-------------------------------------------------
#
# Project created by QtCreator 2014-02-03T13:51:07
#
#-------------------------------------------------

QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OE_Server
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    satdata.cpp \
    parsesatdata.cpp \
    netcatgrabber.cpp \
    tableview.cpp

HEADERS  += dialog.h \
    satdata.h \
    parsesatdata.h \
    Constants.h \
    netcatgrabber.h \
    tableview.h

FORMS    += dialog.ui \
    tableview.ui

RESOURCES += \
    Resources.qrc

OTHER_FILES += \
    ../../../satviewNew/satview/OE_Server/tableview.o
