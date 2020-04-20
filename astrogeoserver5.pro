#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T12:09:35
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = astrogeoserver5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    genericclient.cpp \
    utility.cpp \
    setupgui.cpp \
    querytopo.cpp \
    sunangle.cpp \
    jpleph.cpp \
    genericserver.cpp

HEADERS  += mainwindow.h \
    genericclient.h \
    utility.h \
    setupgui.h \
    querytopo.h \
    sunangle.h \
    jpl_int.h \
    jpleph.h \
    genericserver.h

FORMS    += mainwindow.ui
