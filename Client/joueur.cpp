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
        boatIsHorizontal[i]=true;
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
    for(int boatVerificated=0; boatVerificated<8; boatVerificated++)
    {
        for(int boatVerificatedSize=0; boatVerificatedSize<boatSize[boatVerificated]; boatVerificatedSize++)
        {
            for(int boatChecked=0; boatChecked<8;boatChecked++)
            {
                for(int boatCheckedSize=0; boatCheckedSize<boatSize[boatChecked]; boatCheckedSize++)
                {
                    if(boatVerificated!=boatChecked)
                    {
                        if(boatIsHorizontal[boatVerificated]==true && boatIsHorizontal[boatChecked]==true)
                        {
                            if(boatPosx[boatVerificated]+boatVerificatedSize==boatPosx[boatChecked]+boatCheckedSize && boatPosy[boatVerificated]==boatPosy[boatChecked])
                            {
                                return 1;
                            }
                        }
                        if(boatIsHorizontal[boatVerificated]==false && boatIsHorizontal[boatChecked]==false)
                        {
                            if(boatPosx[boatVerificated]==boatPosx[boatChecked] && boatPosy[boatVerificated]+boatVerificatedSize==boatPosy[boatChecked]+boatCheckedSize)
                            {
                                return 1;
                            }
                        }
                        if(boatIsHorizontal[boatVerificated]==true && boatIsHorizontal[boatChecked]==false)
                        {
                            if(boatPosx[boatVerificated]+boatVerificatedSize==boatPosx[boatChecked] && boatPosy[boatVerificated]==boatPosy[boatChecked]+boatCheckedSize)
                            {
                                return 1;
                            }
                        }
                        if(boatIsHorizontal[boatVerificated]==false && boatIsHorizontal[boatChecked]==true)
                        {
                            if(boatPosx[boatVerificated]==boatPosx[boatChecked]+boatCheckedSize && boatPosy[boatVerificated]+boatVerificatedSize==boatPosy[boatChecked])
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i=0; i<8; i++)//Verification que tous les bateaux sont dans la grille
    {
        if(boatIsHorizontal[i]==true)
        {
            if((boatPosx[i]<0 || boatPosx[i]+boatSize[i]-1>15 || boatPosy[i]<0 || boatPosy[i]>15 )&& doneClicked==true)
            {
                return 2;
            }
        }
        if(boatIsHorizontal[i]==false)
        {
            if((boatPosx[i]<0 || boatPosx[i]>15 || boatPosy[i]<0 || boatPosy[i]+boatSize[i]-1>15 )&& doneClicked==true)
            {
                return 2;
            }
        }
    }
    return 0;
}

QString Joueur::writeMessage()
{
    QString message = "";
    message = message+NewPlayer ;
    message=playerName+":";
    for(int i=0; i<8; i++)
    {
        message=message+QString::number(boatPosx[i])+":"+QString::number(boatPosy[i])+":"+QString::number(boatSize[i])+":"+QString::number(boatIsHorizontal[i])+":";
    }
    return message;
}
