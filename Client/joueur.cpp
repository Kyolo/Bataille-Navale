#include "joueur.h"

Joueur::Joueur(QString name)
{
    playerName=name;
    for(int a=0; a<16; a++)
    {
        for (int b=0; b<16; b++)
        {
            attackIsInTheWater[a][b]=false;
            isAttack[a][b]=false;
        }
    }
    for (int i=0; i<8; i++)
    {
        boatPosx[i]=-1;
        boatPosy[i]=-1;
        boatIsHorizontal[i]=-1;
        boatSize[i]=-1;
    }
}
void Joueur::setPerson(bool isMe)
{
    isLocal=isMe;
}
bool Joueur::getPerson()
{
    return isLocal;
}

void Joueur::setBoat(int posx, int posy, bool isHorizontal, int numberOfBoat, int size)
{
    boatPosx[numberOfBoat]=posx;
    boatPosy[numberOfBoat]=posy;
    boatIsHorizontal[numberOfBoat]=isHorizontal;
    boatSize[numberOfBoat]=size;
}

void Joueur::setAttack(int posx, int posy, bool isInTheWater)
{
    isAttack[posx][posy]=true;
    attackIsInTheWater[posx][posy]=isInTheWater;
}
QString Joueur::getName()
{
    return playerName;
}

int Joueur::checkWrite(bool doneClicked)
{
    for (int i=0; i<8; i++)
    {
        for (int a=0; a<0; a++)
        {
            for (int b=0; b<boatSize[i]; b++)
            {
                if(boatIsHorizontal[i]==true)
                {
                    if(boatPosx[i]+b==boatPosx[a] && boatPosy[i]==boatPosy[a] && i!=a)
                    {
                        return 1;
                    }
                }
                if(boatIsHorizontal[i]==false)
                {
                    if(boatPosx[i]==boatPosx[a] && boatPosy[i]+b==boatPosy[a] && i!=a)
                    {
                        return 1;
                    }
                }
            }
        }
        if((boatPosx[i]<0 || boatPosx[i]>=15 || boatPosy[i]<0 || boatPosy[i]>=15 )&& doneClicked==true)
        {
            return 2;
        }
    }
    return 0;
}
