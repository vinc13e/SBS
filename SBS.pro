TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += thread

TARGET = sbs

CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += main.cpp \
    msmat.cpp \
    poolingbs.cpp \
    spectralangle.cpp

HEADERS += \
    msmat.h \
    poolingbs.h \
    spectralangle.h


QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
