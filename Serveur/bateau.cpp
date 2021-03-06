#include "bateau.h"

//******************************************Constructeur******************************************
Bateau::Bateau(uchar taille, uchar posx, uchar posy, bool hori){
    this->bHorizontal=hori;
    this->statut = new bool[taille];
    this->bX=posx;
    this->bY=posy;
    this->bTaille=taille;

    for(int i=0;i<taille;i++){
        statut[i]=true;
    }
}


//******************************************état du bateau***********************************
/**
 * @brief Joueur::isDestroyed
 * On vérifie si le bateau est détruit, C.A.D que toutes les cases (tableau 'statut') sont touchées
 * @return Si le bateau est détruit
 */
bool Bateau::isDestroyed(){
    for(int i=0;i<bTaille;i++){
        if(statut[i]==true)
            return false;
    }
    return true;
}

/**
 * @brief Bateau::hit : Sert à vérifier si le bateau est touché ET détruit la case correspondante
 * @param x : la coordonnée verticale
 * @param y : la coordonnée verticale
 * @return Si oui ou non le bateau a été touché
 */
bool Bateau::hit(uchar x, uchar y){
    if(this->bHorizontal)
    {
        for(int i=0; i<bTaille;i++)
        {
            if(x==bX+i && y==bY)
            {
                statut[i]=false;
                return true;
            }
        }
        return false;
    }
    else
    {
        for(int i=0; i<bTaille;i++)
        {
            if(x==bX && y==bY+i)
            {
                statut[i]=false;
                return true;
            }
        }
        return false;
    }
}


/**
 * @brief Bateau::destroy : Détruit le bateau, seules utilisation : couroux vengeur de l'admin ou abandon du joueur
 */
void Bateau::destroy(){
    for(int i = 0;i<bTaille;i++)
        statut[i]=false;
}

char Bateau::getStatusAt(uchar x, uchar y){
    if(this->bHorizontal)
    {
        for(int i=0; i<bTaille;i++)
        {
            if(x==bX+i && y==bY)
            {
                if(statut[i])
                return SAFE;
                else
                return DESTROYED;
            }

        }
        return NONE;
    }
    else
    {
        for(int i=0; i<bTaille;i++)
        {
            if(x==bX && y==bY+i)
            {
                if(statut[i])
                return SAFE;
                else
                return DESTROYED;
            }
        }
        return NONE;
    }
}



//*******************************************Différent getter et setter***********************************
uchar Bateau::getPositionX(){
    return this->bX;
}

uchar Bateau::getPositionY(){
    return this->bY;
}

bool Bateau::isHorizontal(){
    return this->bHorizontal;
}

uchar Bateau::getPV(){
    uchar pv=0;
    for(int i=0;i<bTaille;i++){
        if(statut[i]==true)
            pv++;
    }
    return pv;
}
