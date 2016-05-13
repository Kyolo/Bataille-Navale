#include <command.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <QCoreApplication>

#include <joueur.h>
#include <game.h>

using namespace std;

CommandManager::CommandManager(){
    setParent(QCoreApplication::instance());
}

/**
 * @brief CommandManager::useCommand : Sert à traiter des commandes
 * @param command : la commande
 * @param args : les divers parametre de la commande
 */
void CommandManager::useCommand(QString command, QStringList args ){
    if(command=="stop"){
        Game::getInstance()->sendToChat("<u><i><b>Arret du serveur</b></i></u>");
        Game::getInstance()->forceQuit();
        return;
    }else if(command=="say"){
        Game::getInstance()->sendToChat("<i><b>Serveur</b></i> : "+args.join(" "));
        return;
    } else if(command=="help"){
        if(args.length()==0){
            cout<<"Liste des commandes : "<<endl;
            cout<<"\thelp [commande] : Affiche l'aide globale ou d'une fonction"<<endl;
            cout<<"\tsay <message> : Envoie un message aux clients connectés"<<endl;
            cout<<"\tdisp <joueur> : Affiche la grille du (ou des) joueur(s) demandé(s)"<<endl;
            cout<<"\tstop : Force l'arret du serveur et provoque l'abandon forcé des joueurs"<<endl;
            cout<<"\tlist : Affiche la liste des joueurs connectés"<<endl;
            cout<<"\tkick <joueur> : Expulse le joueur désigné"<<endl;
            cout<<"\tkick-clean : Déconnecte tout les clients qui ne sont pas des joueurs"<<endl;
        } else {

        }
        return;
    }
    else if(command=="disp"&&args.length()>0){
        for(int i=0;i<args.length();i++){
            Joueur j = Game::getInstance()->getPlayerByName(args[i]);
            cout<<"Grille du joueur "<<j.getName().toStdString()<<" :"<<endl;
            cout<<"+"<<QString(16,'-').toStdString()<<"+"<<endl;
            for(int y = 0;y<16;y++){
                cout<<"|";
                for(int x = 0;x<16;x++){
                    cout<<j.getStatusAt(x,y);
                }
                cout<<"|"<<endl;
            }
            cout<<"+"<<QString(16,'-').toStdString()<<"+"<<endl;
        }
        return;
    } else if(command=="list"&&args.length()==0){
        int num = 0;
        Joueur * plrs = Game::getInstance()->getAllPlayer();
        num = Game::getInstance()->getNbJoueur();
        cout<<"Les joueurs connectés sont :"<<endl;
        for(int i=0;i<num;i++){
            cout<<"\t"<<plrs[i].getName().toStdString()<<" "<<(plrs[i].areAllBoatsDestroyed()?"Mort":"Lice")<<endl;
        }
        return;
    } else if(command=="kick"&&args.length()>0){
        cout<<"Les joueurs :"<<endl;
        for(int i = 0;i<args.size();i++){
            cout<<"\t"<<args[i].toStdString()<<endl;
            Game::getInstance()->kick(args[i]);
        }
        cout<<"ont été kick"<<endl;
        return;
    } else if(command=="kick-clean"&&args.length()==0){
        cout<<"Nettoyage des clients"<<endl;
        Game::getInstance()->cleanConnection();
    }

    cerr<<"Commande inconnue"<<endl;
}

void CommandManager::run()
{
    forever{
        string command;
        //On lit la commande dans cin
        getline(cin,command);

        //On sépare la commande de ses arguments
        QStringList div = QString(command.c_str()).split(" ");
        QString com = div[0];
        div.removeAt(0);

        //Et on les emets pour traitement dans un thread à part
        emit commandDetected(com,div);

    }
}
