#include "joueur.h"

Joueur::Joueur(Bateau *bateau, QString name, int nbBateau)
{
    this->lstBoat = bateau;
    this->nbBoat = nbBateau;
    this->pseudo = name;
}

/**
 * @brief Joueur::Joueur : à ne pas utiliser !!
 */
Joueur::Joueur(){}

bool Joueur::attack(uchar x, uchar y){
    for(int i = 0;i<this->nbBoat;i++){
        if(this->lstBoat[i].hit(x,y))
            return true;
    }
    return false;
}

bool Joueur::isAllBoatDestroyed(){
    return this->getNbDestroyedBoat()==nbBoat;
}

int Joueur::getNbDestroyedBoat(){
    int count = 0;
    for(int i=0;i<nbBoat;i++){
        if(lstBoat[i].isDestroyed())
            count++;
    }
    return false;
}

void Joueur::giveUp(){
    for(int i=0;i<nbBoat;i++){
        lstBoat[i].destroy();
    }
}
