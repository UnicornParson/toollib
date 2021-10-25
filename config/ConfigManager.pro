#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T14:40:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ConfigManager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(ConfigManager.pri)

SOURCES += main.cpp

Release:DESTDIR = bin/release
Release:OBJECTS_DIR = bin/release/.obj
Release:MOC_DIR = bin/release/.moc
Release:RCC_DIR = bin/release/.rcc
Release:UI_DIR = bin/release/.ui

Debug:DESTDIR = bin/debug
Debug:OBJECTS_DIR = bin/debug/.obj
Debug:MOC_DIR = bin/debug/.moc
Debug:RCC_DIR = bin/debug/.rcc
Debug:UI_DIR = bin/debug/.ui
