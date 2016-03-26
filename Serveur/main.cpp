#include <QtCore>

#include <game.h>

int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);

    Game * gme = new Game();

    gme->start();

    return a.exec();
}
