#include <stdio.h>
#include <iostream>
#include <string.h>
#include <QString>

#include "game.h"

using namespace std;

int main(int argc, char *argv[]){

    Game * gme;

    if(argc>1){
        if(argv[1]==string("help")){
            cout<<"Utilisation : bnserveur [nombreJoueur] [port]"<<endl;
            return 0;
        }
        else {
            if(QString(argv[1]).toInt()>0){
                gme = new Game(QString(argv[1]).toInt());
            } else {
                gme = new Game;
            }
        }
    }

    if(gme==0)
        return 0;

    gme->start();
    gme->loop();

}
