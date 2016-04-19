#ifndef ADVERSAIRE_H
#define ADVERSAIRE_H

#include <QString>

class Adversaire
{
public:
    Adversaire(QString setName);
    QString getName();
    void attack(int x, int y, bool isInTheWater);
    int getSituation(int x, int y);
private:
    QString name;
    int isInWater[16][16];
};

class AdversaireTab
{
public:
    AdversaireTab(int numberOfAdversaire);
    int getByName(QString name);
    Adversaire *adversaire;
private:
    int nbr;
};



#endif // ADVERSAIRE_H