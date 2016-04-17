#include "adversaire.h"

Adversaire::Adversaire(QString setName)
{
    for (int a=0; a<16; a++)
    {
        for (int b=0; b<16; b++)
        {
            isInWater[a][b]=2;
        }
    }
    name=setName;
}

QString Adversaire::getName()
{
    return name;
}

void Adversaire::attack(int x, int y, bool isInTheWater)
{
    isInWater[x][y]=isInTheWater;
}

int Adversaire::getSituation(int x, int y)
{
    return isInWater[x][y];
}

AdversaireTab::AdversaireTab(int numberOfAdversaire)
{
    nbr=numberOfAdversaire;
    //adversaire = new Adversaire[nbr];
}

int AdversaireTab::getByName(QString name)
{
    for(int i=0; i<nbr; i++)
    {

    }
}
