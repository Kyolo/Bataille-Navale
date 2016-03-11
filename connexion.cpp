#include "connexion.h"
#include <QtMath>


Joueur *Connexion::waitForJoueur(int nbJoueur, int maxBateau, int * sizeBateau){
    qsrand(65565);
    Joueur * toRet = new Joueur[nbJoueur];
    for(int i=0;i<nbJoueur;i++){
        Bateau * boat = new Bateau[7];
        for(int j=0;i<maxBateau;i++){
            boat[j]= Bateau(sizeBateau[j],qrand()%16,qrand()%16,0);
        }
        toRet[i] = Joueur(boat);
    }
    return toRet;
}

char *Connexion::recevoirInfo()
{
    return 0;
}

void Connexion::sendInfo()
{

}
