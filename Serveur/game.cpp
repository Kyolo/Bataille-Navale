#include "game.h"

#include <iostream>
#include <stdio.h>

using namespace std;

Game::Game(uchar nbPlr){
    nbJoueurMax=nbPlr;
    nbJoueurCo=0;
    nbJoueurEnLice=0;
    co = new Connexion();
    QObject::connect(co,SIGNAL(connexionNvJoueur(Joueur)),this,SLOT(newPlayer(Joueur)));
    QObject::connect(this,SIGNAL(gameStarted()),co,SLOT(gameStarted()));
    QObject::connect(co,SIGNAL(attaque(QString,QString,uchar,uchar)),this,SLOT(onAttack(QString,QString,uchar,uchar)));
    QObject::connect(this,SIGNAL(attackResult(QString,uchar,uchar,bool)),co,SLOT(attackResult(QString,uchar,uchar,bool)));
    QObject::connect(this,SIGNAL(playerLost(QString)),co,SLOT(playerLost(QString)));
    QObject::connect(co,SIGNAL(playerGiveUp(QString)),this,SLOT(giveUp(QString)));
}

void Game::start(){
    cout<<"En attente de "<<this->nbJoueurMax<<" joueurs"<<endl;

}

void Game::loop(){


}

Joueur Game::getPlayerByName(QString name){
    for(int i = 0;i<nbJoueurCo;i++){
        if(lstJoueur[i].getName()==name)
            return lstJoueur[i];
    }

    return NULL;
}

Game::~Game(){
    delete co;
}

//****************************************Slots************************************************************
void Game::newPlayer(Joueur j){
    this->lstJoueur[nbJoueurCo]=j;
    cout<<j.getName().toStdString().c_str()<<" vient de se connecter. Il faut encore "<<nbJoueurMax-nbJoueurCo<<" joueurs avant le début de la partie."<<endl;
    nbJoueurCo++;
    nbJoueurEnLice++;
    if(nbJoueurCo==nbJoueurMax){
        cout<<"Tout les joueurs sont connectés, début de la partie"<<endl;
        emit gameStarted();
    }
}

void Game::onAttack(QString from, QString to, uchar x, uchar y){
    //On attaque le joueur correspondant à la case correspondante
    bool hit = this->getPlayerByName(to).attack(x,y);

    //Et on envoit les résultat aux deux joueurs concernés
    emit attackResult(to,x,y,hit);
    emit attackResult(from,x,y,hit);

    //Si le joueur attaqué a perdu
    if(this->getPlayerByName(to).isAllBoatDestroyed()){
        emit playerLost(to);//On prévient les autres joueurs
        nbJoueurEnLice--;//Et on décrémente le nombre de joueur en lice
    }

    //S'il ne reste plus qu'un joueur, c'est le gagnant
    if(nbJoueurEnLice==1){
        QString winner;

        for(int i=0;i<nbJoueurMax;i++){
            if(!lstJoueur[i].isAllBoatDestroyed())//Par définition c'est celui qui n'a pas tout ses bateaux de détruits
                winner=lstJoueur[i].getName();
        }

        emit gameFinished(winner);
    }

}

void Game::giveUp(QString who){
    this->getPlayerByName(who).giveUp();
    emit playerLost(who);
}
