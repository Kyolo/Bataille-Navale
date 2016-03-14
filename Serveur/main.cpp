#include <stdio.h>
#include <iostream>
#include <string.h>
#include <QString>

#include "game.h"

using namespace std;

int main(int argc, char *argv[]){

    Game * gme = new Game();

    gme->start();
    gme->loop();

}
