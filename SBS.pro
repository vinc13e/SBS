TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = sbs

CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += main.cpp
