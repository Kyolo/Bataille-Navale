#ifndef MESSAGEGESTION_H
#define MESSAGEGESTION_H

#include <QString>
#include <QObject>

#include <stdio.h>
#include <iostream>

#include "joueur.h"
#include "bateau.h"



class messageGestion : public QObject
{
    Q_OBJECT
public:
    messageGestion();
    void inputMessage(QString message);
signals:
    void connexionNvJoueur(Joueur);
    void attaque(QString,QString,uchar,uchar);
    void playerGiveUp(QString);
    void tchat(QString);
};

#endif // MESSAGEGESTION_H
