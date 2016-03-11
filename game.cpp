#include "game.h"
#include "connexion.h"

Game::Game(uchar nbPlr){
    nbJoueur=nbPlr;
}

void Game::start(){
    this->lstJoueur = Connexion::waitForJoueur(nbJoueur,3,new int[]{1,2,3});
}

void Game::loop(){
    int joueurEnLice = nbJoueur;

    do{
        char * data = Connexion::recevoirInfo();
        traiterInfo();
        joueurEnLice=nbJoueur;
        foreach(Joueur j,lstJoueur){
            if(j.isAllBoatDestroyed())
                joueurEnLice--;
        }

        Connexion::sendInfo();

    }while(joueurEnLice>1);
}

void Game::traiterInfo(){

}
