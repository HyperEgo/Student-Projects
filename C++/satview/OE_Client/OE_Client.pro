#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T18:13:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT+=sql
QT+=gui

TARGET = OE_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    satdata.cpp \
    customlabel.cpp \
    llfromxy.cpp \
    satellite.cpp \
    satellitemanager.cpp \
    sgp4.cpp \
    animatedialog.cpp \
    animated2.cpp \
    customlabelpix.cpp \
    customgraphicsscene.cpp

HEADERS  += mainwindow.h \
    satdata.h \
    customlabel.h \
    llfromxy.h \
    satellite.h \
    satellitemanager.h \
    sgp4.h \
    animatedialog.h \
    animated2.h \
    customlabelpix.h \
    customgraphicsscene.h

FORMS    += mainwindow.ui \
    animatedialog.ui \
    animated2.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    images/FTDX3000-scope.jpg \
    images/oe_client_logo.jpg \
    images/starry_sky.jpg \
    images/cropped_mercator_no_degrees.png \
    images/test_starry_back.png \
    images/test_starry_night_back.png \
    images/usa_mercator_map_no_degrees.png \
    images/satpics/sat001.png \
    images/satpics/sat002.png \
    images/satpics/sat003.png \
    images/satpics/sat004.png \
    images/satpics/sat005.png \
    images/satpics/sat006.png \
    images/satpics/sat007.png \
    images/satpics/sat008.png \
    images/satpics/sat009.png \
    images/satpics/sat010.png \
    images/satpics/sat011.png \
    images/satpics/sat012.png \
    images/satpics/sat013.png \
    images/satpics/sat014.png \
    images/satpics/sat015.png \
    images/satpics/sat016.png \
    images/satpics/sat017.png \
    images/satpics/sat018.png \
    images/satpics/sat019.png \
    images/satpics/sat020.png
