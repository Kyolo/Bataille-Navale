#ifndef JOUEUR_H
#define JOUEUR_H

#include <bateau.h>
#include <QString>

typedef unsigned char uchar;

class Joueur
{
public:
    Joueur(Bateau * bateau,QString name = "none", int nbBateau = 7);
    Joueur();
    bool attack(uchar x, uchar y);
    bool isAllBoatDestroyed();
    int getNbDestroyedBoat();

private:
    Bateau * lstBoat;
    int nbBoat;
    QString pseudo;
};

#endif // JOUEUR_H
