#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T14:04:19
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = IGbataillenavale
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    joueur.cpp \
    adversaire.cpp

HEADERS  += mainwindow.h \
    client.h \
    comunicationconstants.h \
    joueur.h \
    adversaire.h

FORMS    += mainwindow.ui

RESOURCES += \
    Images/images.qrc \
    Sons/sons.qrc

DISTFILES += \
    Sons/sonchat.wav \
    Images/mer.gif \
    Images/carteFond.jpg \
    Images/carteFondFlou.jpg \
    Images/Damiereau.jpg \
    Images/imageTransparente.png \
    Sons/sonClique.wav \
    Sons/sonDebutPartie.wav \
    Sons/sonDefaite.wav \
    Sons/sonVictoire.wav
