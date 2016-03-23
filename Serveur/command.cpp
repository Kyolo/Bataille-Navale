#include  <command.h>

#include <iostream>
#include <stdio.h>

#include <joueur.h>

using namespace std;

void Command::doCommand(QString command, QStringList args ){
    if(command=="stop"){
        Game::getInstance()->sendToChat("Arret du serveur");
        Game::getInstance()->forceQuit();
    }else if(command=="say"){
        Game::getInstance()->sendToChat(args.join(" "));
    } else if(command=="help"){
        if(args.length()==0){
            cout<<"Liste des commandes : "<<endl;
            cout<<"\thelp [commande] : affiche l'aide globale ou d'une fonction"<<endl;
            cout<<"\tsay <message> : Envoie un message aux clients connectés"<<endl;
            cout<<"\tdisp <joueur> : Affiche la grille du joueur demandé"<<endl;
            cout<<"\tstop : Force l'arret du serveur et provoque l'abandon forcé des joueurs"<<endl;
        }
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
    }
}
