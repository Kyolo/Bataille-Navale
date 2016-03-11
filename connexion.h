#ifndef CONNEXION_H
#define CONNEXION_H

#include <joueur.h>

class Connexion
{
public:
    static Joueur* waitForJoueur(int nbJoueur, int maxBateau, int *sizeBateau);
    static char * recevoirInfo();
    static void sendInfo();
};

#endif // CONNEXION_H
