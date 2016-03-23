#include "game.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <command.h>

Game * Game::instance=NULL;

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
    QObject::connect(this,SIGNAL(gameFinished(QString)),co,SLOT(playerWon(QString)));
    run = true;
}

void Game::start(){
    cout<<"En attente de "<<this->nbJoueurMax<<" joueurs"<<endl;
}

/**
 * @brief Game::loop : la boucle qui gère la lecture et l'utilisation des commandes
 */
void Game::loop(){
    //Ici viens la gestion des commandes
    while(run){

        string command;

        //On lit la commande dans cin
        getline(cin,command);

        //On sépare la commande de ses arguments
        QStringList div = QString(command.c_str()).split(" ");
        QString com = div[0];
        div.removeAt(0);

        //Et on les envoie pour traitement
        Command::doCommand(com,div);

    }

}

/**
 * @brief Game::getPlayerByName : Permet d'obtenir un joueur en fonction de son pseudo, évite une répétition de code
 * @param name : Le nom du joueur
 * @return l'objet joueur correspondant ou NULL le cas échéant.
 */
Joueur Game::getPlayerByName(QString name){
    for(int i = 0;i<nbJoueurCo;i++){
        if(lstJoueur[i].getName()==name)
            return lstJoueur[i];
    }

    return NULL;
}

/**
 * @brief Game::forceQuit : Force l'arrêt du serveur et envoie un signal d'abandon pour tout les joueurs
 */
void Game::forceQuit(){
    for(int i = 0;i<nbJoueurCo;i++){
        lstJoueur[i].giveUp();
        emit playerLost(lstJoueur[i].getName());
    }
    run=false;
}

void Game::sendToChat(QString msg){
    co->sendtoclient(msg);
}

Game::~Game(){
    delete co;
}

void Game::setInstance(Game *gme){
    Game::instance=gme;
}

Game *Game::getInstance(){
    return Game::instance;
}

//****************************************Slots************************************************************
/**
 * @brief Game::newPlayer : Gère l'ajout d'un nouveau joueur
 * @param j : l'objet joueur correspondant
 */
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

/**
 * @brief Game::onAttack : gère l'attaque d'un joueur contre un autre
 * @param from : L'attaquant
 * @param to : Le défenseur
 * @param x : la coordonée x
 * @param y : la coordonée y
 */
void Game::onAttack(QString from, QString to, uchar x, uchar y){
    //On attaque le joueur correspondant à la case correspondante
    bool hit = this->getPlayerByName(to).attack(x,y);

    //Et on envoit les résultat aux deux joueurs concernés
    emit attackResult(to,x,y,hit);
    emit attackResult(from,x,y,hit);

    //Si le joueur attaqué a perdu
    if(this->getPlayerByName(to).areAllBoatsDestroyed()){
        emit playerLost(to);//On prévient les autres joueurs
        nbJoueurEnLice--;//Et on décrémente le nombre de joueur en lice
    }

    //S'il ne reste plus qu'un joueur, c'est le gagnant
    if(nbJoueurEnLice==1){
        QString winner;

        for(int i=0;i<nbJoueurMax;i++){
            if(!lstJoueur[i].areAllBoatsDestroyed())//Par définition c'est celui qui n'a pas tout ses bateaux de détruits
                winner=lstJoueur[i].getName();
        }
        //Et on préviens les autres qu'on a finit
        emit gameFinished(winner);
    }

}

/**
 * @brief Game::giveUp : gère l'abandon d'un joueur
 * @param who : le couard qui abandonne
 */
void Game::giveUp(QString who){
    this->getPlayerByName(who).giveUp();
    emit playerLost(who);
}
