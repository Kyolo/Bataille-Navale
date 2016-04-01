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
}
void Joueur::setPerson(bool isMe)
{
    isLocal=isMe;
}
bool Joueur::getPerson()
{
    return isLocal;
}

void Joueur::setBoat(int posx, int posy, bool isHorizontal, int numberOfBoat)
{
    boatPosx[numberOfBoat]=posx;
    boatPosy[numberOfBoat]=posy;
    boatIsHorizontal[numberOfBoat]=isHorizontal;
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
