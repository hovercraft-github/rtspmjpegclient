#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T10:31:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mjpegrtspclientlibtestqt
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        rtspmjpegclienttestmainwindow.cpp

HEADERS += rtspmjpegclienttestmainwindow.h

FORMS += rtspmjpegclienttestmainwindow.ui

CONFIG += c++11

unix {
    INCLUDEPATH += ../../include
    LIBS += -L../build-rtspmjpegclient-Desktop-Debug -lrtspmjpegclient
}
