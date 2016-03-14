#ifndef GAME_H
#define GAME_H

#include <joueur.h>
#include <connexion.h>

#include <QObject>
#include <QString>

typedef unsigned char uchar;

class Game: public QObject
{
    Q_OBJECT
public:
    Game(uchar nbPlr = 2);
    void start();
    void loop();
    Joueur getPlayerByName(QString name);

private:
    Joueur * lstJoueur;
    int nbJoueurMax;
    int nbJoueurCo;
    int nbJoueurEnLice;
    Connexion * co;
    ~Game();


private slots:
    void newPlayer(Joueur j);
    void onAttack(QString from,QString to,uchar x, uchar y);
    void giveUp(QString who);

signals:
    void gameStarted();
    void attackResult(QString,uchar,uchar,bool);
    void playerLost(QString);
    void gameFinished(QString);
};

#endif // GAME_H
