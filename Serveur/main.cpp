#include <stdio.h>
#include <iostream>
#include <string.h>
#include <QString>

#include <QtCore>
#include "game.h"

using namespace std;

int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);
    Game * gme = new Game();

    gme->start();
    //gme->loop();
    return a.exec();
}
