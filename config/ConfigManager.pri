#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T14:40:49
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = ConfigManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include($$PWD/../tools/tools.pri)
SOURCES += \
    $$PWD/configmanager.cpp

HEADERS += \
    $$PWD/configmanager.h
