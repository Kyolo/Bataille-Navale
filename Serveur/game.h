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
    void forceQuit();
    void sendToChat(QString msg);
    ~Game();

    static void setInstance(Game * gme);
    static Game *getInstance();
    static Game * instance;

private:
    Joueur * lstJoueur;
    int nbJoueurMax;
    int nbJoueurCo;
    int nbJoueurEnLice;
    Connexion * co;
    bool run;


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
