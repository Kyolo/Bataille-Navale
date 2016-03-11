#ifndef GAME_H
#define GAME_H

#include <joueur.h>

typedef unsigned char uchar;

class Game
{
public:
    Game(uchar nbPlr = 2);
    void start();
    void loop();
    void traiterInfo();

private:
    Joueur * lstJoueur;
    int nbJoueur;
};

#endif // GAME_H
