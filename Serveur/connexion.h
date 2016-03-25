#ifndef CONNEXION_H
#define CONNEXION_H

#include <joueur.h>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#include <string.h>

#include "messagegestion.h"

typedef unsigned char uchar;

class Connexion: public QObject
{
    Q_OBJECT
public:
    Connexion();
    std::string sendtoclient(const QString &message);

private slots:  //les slots de gestion des évènements
    void connexion();
    void getdata();
    void deconnexion();
    QString getIPaddress();
    //slots de gestion de messages
    void SconnexionNvJoueur(Joueur player);
    void Sattaque(QString from,QString to,uchar posx,uchar posy);
    void SplayerGiveUp(QString player);
    void tchat(QString message);

private:
    QTcpServer *server;
    QList<QTcpSocket *> client;
    quint16 tailleMessage;
    messageGestion *msgGest;

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
