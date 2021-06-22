#-------------------------------------------------
#
# Project created by QtCreator 2013-11-26T02:41:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT+=sql
QT+=gui

TARGET = project03
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    customlabel.cpp \
    customgraphicsscene.cpp \
    customgraphicspixmapitem.cpp

HEADERS  += dialog.h \
    customlabel.h \
    customlabel.h \
    customgraphicsscene.h \
    customgraphicspixmapitem.h

FORMS    += dialog.ui
