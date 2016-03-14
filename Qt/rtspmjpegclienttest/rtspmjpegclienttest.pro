TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

SOURCES += ../../src/main.cpp

unix {
    INCLUDEPATH += ../../include
    LIBS += -L../build-rtspmjpegclient-Desktop-Debug -lrtspmjpegclient
}
