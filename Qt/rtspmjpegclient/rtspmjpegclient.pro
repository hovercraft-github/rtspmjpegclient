TEMPLATE = lib
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../include

HEADERS += ../../include/loghandler.hpp \
    ../../include/rtspmjpegclient.hpp \
    ../../include/rtspmjpegclient_common.hpp \
    ../../include/rtspmjpegclient_interface.hpp

SOURCES += ../../src/loghandler.cpp \
    ../../src/rtspmjpegclient.cpp \
    ../../src/rtspmjpegclient_interface.cpp

unix {
    CONFIG += link_pkgconfig
    LIBS = -llog4cplus -lavutil -lavformat -lavcodec
}
