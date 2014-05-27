#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T22:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neural_network
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    neural_network.cpp \
    draw.cpp \
    tread.cpp

HEADERS  += mainwindow.h \
    neural_network.h \
    tread.h

FORMS    += mainwindow.ui
