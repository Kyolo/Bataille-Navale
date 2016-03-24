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
    return false;
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
    QStringList lst = QStringList();//On crée une liste

    for(int i=0;i<nbBoat;i++){
        lst.push_back(QString(QChar(lstBoat->getStatusAt(x,y))));//On y ajoute le statut de tout les bateaux
    }

    lst.removeDuplicates();//On enlève les doublons, car il y aura forcement plein d'espaces

    if(lst.length()==0)
        return Bateau::NONE;

    return lst.at(0);

}
