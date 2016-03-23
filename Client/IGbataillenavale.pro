#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T14:04:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IGbataillenavale
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h

FORMS    += mainwindow.ui

RESOURCES += \
    Images/Images.qrc

DISTFILES += \
    Sons/sonchat.wav \
    Images/mer.gif \
    Images/carteFond.jpg \
    Images/carteFondFlou.jpg \
    Images/Damiereau.jpg \
    Images/imageTransparente.png
