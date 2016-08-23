#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T16:41:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = perlin_noise
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    perlinnoise1d.cpp \
    perlinnoise2d.cpp

HEADERS  += widget.h \
    perlinnoise1d.h \
    perlinnoise2d.h

FORMS    += widget.ui
