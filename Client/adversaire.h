#ifndef ADVERSAIRE_H
#define ADVERSAIRE_H

#include "QString"

class Adversaire
{
public:
    Adversaire(QString name="none");
    unsigned int grille[16][16];
    int getState(int x, int y);
    QString name;
    void setState(int x, int y, bool isInTheWater);
};

class TableauAdversaire
{
public:
    TableauAdversaire();
    Adversaire *adv;
    void newAdv(QString name);
    int getByName(QString name);
};

#endif // ADVERSAIRE_H
