#include "game.h"

Game::Game(uchar nbPlr){
    nbJoueur=nbPlr;
    co = new Connexion();
    QObject::connect(co,SIGNAL(connexionNvJoueur(Joueur)),this,SLOT(newPlayer(Joueur)));
    QObject::connect(this,SIGNAL(gameStarted()),co,SLOT(gameStarted()));
    QObject::connect(co,SIGNAL(attaque(QString,QString,uchar,uchar)),this,SLOT(onAttack(QString,QString,uchar,uchar)));
    QObject::connect(this,SIGNAL(attackResult(QString,uchar,uchar,bool)),co,SLOT(attackResult(QString,uchar,uchar,bool)));
    QObject::connect(this,SIGNAL(playerLost(QString)),co,SLOT(playerLost(QString)));
    QObject::connect(co,SIGNAL(playerGiveUp(QString)),this,SLOT(giveUp(QString)));
}

void Game::start(){
    this->lstJoueur = NULL;//Connexion::waitForJoueur(nbJoueur,3,new int[]{1,2,3});

}

void Game::loop(){
    int joueurEnLice = nbJoueur;

    do{
        //char * data = Connexion::recevoirInfo();
        traiterInfo();
        joueurEnLice=nbJoueur;
        for(int i = 0;i<nbJoueur;i++){
            if(lstJoueur[i].isAllBoatDestroyed())
                joueurEnLice--;
        }
        //Connexion::sendInfo();

    }while(joueurEnLice>1);
}

void Game::traiterInfo(){

}

Game::~Game(){
    delete co;
}

//****************************************Slots************************************************************
void Game::newPlayer(Joueur j){

}

void Game::onAttack(QString from, QString to, uchar x, uchar y){

}

void Game::giveUp(QString who){

}
