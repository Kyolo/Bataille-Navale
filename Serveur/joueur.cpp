#include "joueur.h"

#include <QStringList>
#include <QChar>

Joueur::Joueur(Bateau *bateau, QString name, int nbBateau){
    this->lstBoat = bateau;
    this->nbBoat = nbBateau;
    this->pseudo = name;
}

bool Joueur::attack(uchar x, uchar y){
    for(int i = 0;i<this->nbBoat;i++){
        if(this->lstBoat[i].hit(x,y))
            return true;
    }
    return false;
}

bool Joueur::areAllBoatsDestroyed(){
    return this->getNbDestroyedBoat()==nbBoat;
}

int Joueur::getNbDestroyedBoat(){
    int count = 0;
    for(int i=0;i<nbBoat;i++){
        if(lstBoat[i].isDestroyed())
            count++;
    }
    return count;
}

void Joueur::giveUp(){
    for(int i=0;i<nbBoat;i++){
        lstBoat[i].destroy();
    }
}

QString Joueur::getName(){
    return pseudo;
}

char Joueur::getStatusAt(uchar x, uchar y){

    for(int i=0;i<nbBoat;i++){
        char ch = lstBoat[i].getStatusAt(x,y);//Pour chaque bateau on récupère le statut à sa position
        if(ch!=Bateau::NONE)//Si le statut n'est pas nul, alors il y a un bateau, on renvoit
            return ch;
    }
    
//S'il n'y a rien, on renvoie NONE
    return Bateau::NONE;

}
