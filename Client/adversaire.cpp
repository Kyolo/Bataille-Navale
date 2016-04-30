#include "adversaire.h"

Adversaire::Adversaire(QString name)
{
    this->name=name;
    for (int a=0; a<16; a++)
    {
        for(int b=0; b<16; b++)
        {
            grille[a][b]=2;
        }
    }
}

int Adversaire::getState(int x, int y)
{
    return grille[x][y];
}

void Adversaire::setState(int x, int y, bool isInTheWater)
{
    grille[x][y]=isInTheWater;
}

TableauAdversaire::TableauAdversaire()
{
    adv = new Adversaire[10];
}

void TableauAdversaire::newAdv(QString name)
{
    int newNumber;
    for (int i=0; i<10; i++)
    {
        if(adv[i].name=="none")
        {
            newNumber=i;
            break;
        }
    }
    adv[newNumber]= Adversaire(name);
}

int TableauAdversaire::getByName(QString name)
{
    bool found=false;
    for (int i=0; i<10; i++)
    {
        if(adv[i].name==name)
        {
            bool found =true;
            return i;
        }
    }
    if(found==false)
    {
        return -1;
    }
}

