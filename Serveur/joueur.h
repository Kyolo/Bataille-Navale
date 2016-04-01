#ifndef JOUEUR_H
#define JOUEUR_H

#include <bateau.h>
#include <QString>

typedef unsigned char uchar;

class Joueur
{
public:
    Joueur(Bateau * bateau=NULL,QString name = "none", int nbBateau = DEFAULT_BOAT_NUMBER);
    bool attack(uchar x, uchar y);
    bool areAllBoatsDestroyed();
    int getNbDestroyedBoat();
    void giveUp();
    QString getName();
    char getStatusAt(uchar x, uchar y);
    static const int DEFAULT_BOAT_NUMBER = 8;

private:
    Bateau * lstBoat;
    int nbBoat;
    QString pseudo;
};

#endif // JOUEUR_H
