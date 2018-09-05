#-------------------------------------------------
#
# Project created by QtCreator 2017-11-03T09:10:17
#
#-------------------------------------------------

QT       += core sql
QT       -= gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

TARGET = Report
TEMPLATE = app

contains(DEFINES,QTRPT_LIBRARY) {
    INCLUDEPATH += $$PWD/../../../QtRptProject/QtRPT/
    LIBS += -L$${DEST_DIRECTORY}/lib -lQtRPT
}

# PDF Generator
include($$PWD/../../../QtRptProject/QtRPT/QtRPT.pri)
# Excel Generator
include($$PWD/../../../QtXlsxWriter/src/xlsx/qtxlsx.pri)
#DESTDIR = $${DEST_DIRECTORY}


SOURCES += main.cpp\
        mainwindow.cpp \
    mysql.cpp

HEADERS  += mainwindow.h \
    mysql.h

#FORMS    += mainwindow.ui
#LIBS += -L/usr/local/lib -lpoppler-qt5
#SUBDIRS += 3rdparty/QGumboParser/QGumboParser
