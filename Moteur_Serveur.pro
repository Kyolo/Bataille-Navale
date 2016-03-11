#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T14:20:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Moteur_Serveur
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    bateau.cpp \
    game.cpp \
    connexion.cpp \
    joueur.cpp

HEADERS += \
    bateau.h \
    game.h \
    connexion.h \
    joueur.h

RESOURCES += \
    qsdf.qrc
