#ifndef GAME_H
#define GAME_H

#include <joueur.h>
#include <connexion.h>
#include <command.h>

#include <QObject>
#include <QString>

typedef unsigned char uchar;

class Game: public QObject
{
    Q_OBJECT
public:
    Game(int nbPlr = DEFAULT_PLR_NUMBER);
    void start();
    Joueur getPlayerByName(QString name);
    Joueur *getAllPlayer();
    void forceQuit();
    void sendToChat(QString msg);
    ~Game();
    int getNbJoueur();

    static void setInstance(Game * gme);
    static Game *getInstance();
    static Game * instance;
    static const int DEFAULT_PLR_NUMBER = 2;

private:
    Joueur * lstJoueur;
    int nbJoueurMax;
    int nbJoueurCo;
    int nbJoueurEnLice;
    Connexion * co;
    CommandManager * comManager;


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
