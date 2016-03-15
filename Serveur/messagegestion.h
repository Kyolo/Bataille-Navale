#ifndef MESSAGEGESTION_H
#define MESSAGEGESTION_H

#include <QString>
#include <QObject>
#include "joueur.h"

class messageGestion : public QObject
{
    Q_OBJECT
public:
    void inputMessage(QString message);
signals:
    void connexionNvJoueur(Joueur);
    void attaque(QString,QString,uchar,uchar);
    void playerGiveUp(QString);
    void tchat(QString);
};

#endif // MESSAGEGESTION_H
