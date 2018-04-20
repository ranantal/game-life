#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T22:49:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Life
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    settings.cpp \
    drawpanelwidget.cpp \
    gamemodel.cpp

HEADERS  += mainwindow.h \
    settings.h \
    settingsdialog.h \
    drawpanelwidget.h \
    gamemodel.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui
