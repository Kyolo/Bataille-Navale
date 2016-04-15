#ifndef JOUEUR_H
#define JOUEUR_H

#include <QString>
#include "comunicationconstants.h"

class Joueur
{
public:
    Joueur(QString name);
    QString playerName;
    void setPerson(bool isMe);
    bool getPerson();
    void setBoat(int posx, int posy, bool isHorizontal, int numberOfBoat, int size);
    void setAttack(int posx, int posy, bool isInTheWater);
    QString getName();
    int boatPosx[8];
    int boatPosy[8];
    int boatSize[8];
    bool boatIsHorizontal[8];
    bool isAttack[16][16];
    bool attackIsInTheWater[16][16];
    int checkWrite(bool doneClicked);
    QString writeMessage();
private:
    bool isLocal=false;
};

#endif // JOUEUR_H
