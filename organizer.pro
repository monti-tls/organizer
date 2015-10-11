#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T18:32:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = organizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    task.cpp \
    taskmodel.cpp \
    taskdelegate.cpp \
    taskview.cpp \
    taskwizard.cpp

HEADERS  += mainwindow.h \
    task.h \
    taskmodel.h \
    taskdelegate.h \
    taskview.h \
    taskwizard.h

FORMS    += mainwindow.ui \
    taskwizard.ui
