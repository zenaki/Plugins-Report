#-------------------------------------------------
#
# Project created by QtCreator 2017-11-03T09:10:17
#
#-------------------------------------------------

QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

TARGET = Report_Gui
TEMPLATE = app

contains(DEFINES,QTRPT_LIBRARY) {
    INCLUDEPATH += $$PWD/../../../QtRptProject/QtRPT/
    LIBS += -L$${DEST_DIRECTORY}/lib -lQtRPT
}

include($$PWD/../../../QtRptProject/QtRPT/QtRPT.pri)
#DESTDIR = $${DEST_DIRECTORY}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
