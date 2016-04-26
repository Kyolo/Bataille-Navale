#ifndef CONNEXION_H
#define CONNEXION_H

#include <joueur.h>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#include <string.h>

typedef unsigned char uchar;

class Connexion: public QObject
{
    Q_OBJECT
public:
    Connexion();
    void sendtoclient(const QString &message);
    void sendToOneClient(const QString &message, int whichClient);

private slots:  //les slots de gestion des évènements
    void connexion();
    void getdata();
    void deconnexion();
    QString getIPaddress();
    //slots de gestion de messages
    void tchat(QString message);

private:
    QTcpServer *server;
    QList<QTcpSocket *> client;
    quint16 tailleMessage;
    void messageGestion(QString message);
     Bateau *boats[8];
     Joueur *player;
     QString names[10];

signals:
    void connexionNvJoueur(Joueur);
    void attaque(QString,QString,uchar,uchar);
    void playerGiveUp(QString);

public slots:
    void gameStarted();
    void attackResult(QString who,uchar wherex,uchar wherey, bool in_the_water);
    void playerLost(QString who);
    void playerWon(QString winner);

};

#endif // CONNEXION_H
