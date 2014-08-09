#-------------------------------------------------
#
# Project created by QtCreator 2014-08-09T12:28:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visitsqt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    visitsmodel.cpp \
    visit.cpp \
    visitdelegate.cpp

HEADERS  += mainwindow.h \
    visitsmodel.h \
    visit.h \
    visitdelegate.h

FORMS    += mainwindow.ui
