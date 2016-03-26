#ifndef JOUEUR_H
#define JOUEUR_H

#include <bateau.h>
#include <QString>

typedef unsigned char uchar;

class Joueur
{
public:
    Joueur(Bateau * bateau=NULL,QString name = "none", int nbBateau = 7);
    bool attack(uchar x, uchar y);
    bool areAllBoatsDestroyed();
    int getNbDestroyedBoat();
    void giveUp();
    QString getName();
    char getStatusAt(uchar x, uchar y);

private:
    Bateau * lstBoat;
    int nbBoat;
    QString pseudo;
};

#endif // JOUEUR_H
