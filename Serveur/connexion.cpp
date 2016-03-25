#include "connexion.h"


#include <QTcpServer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QDataStream>

#include <stdio.h>
#include <iostream>
#include <QString>

using namespace std;

Connexion::Connexion()
{
    cout<<"initialisation du serveur..."<<endl;
    //------- Gestion du serveur ------------------------------------------------------
        server=new QTcpServer(this);
        if (!server->listen(QHostAddress::Any, 40110)) //utilisation du port 40110 pour le serveur
        { //erreur lors du démarrage du serveur
            cout<<"Le serveur n'a pas pu être démarré :"<<
                   server->errorString().toStdString()<<endl;
        }
        else
        { //le serveur a réussi à s'initialiser
            cout<<"Le serveur demarre:"<<endl<< "adresse: "<<getIPaddress().toStdString()<<endl<<
                   "port: "<<server->serverPort()<<endl;
            //on autorise la connexion d'un client. S'il ne doit y avoir qu'un seul client il faudra modifier cette ligne
            connect(server, SIGNAL(newConnection()),this, SLOT(connexion()));
        }
        tailleMessage = 0;//comme il n'y a pas encore eu de message la taille du message est nulle
    }
//*************** Rend l'adresse IP *******************************************************************************

QString Connexion::getIPaddress()
{//On part du principe que le réseau est en 192.168.x.y
    QStringList adresse;
    QString resultat;
    foreach(QHostAddress address, QNetworkInterface::allAddresses())
//allAddresses rend toutes les adresses IP. Il faut filtrer
    {
        if (address.isInSubnet(QHostAddress::parseSubnet("192.168.0.0/16")))
        {
            resultat = address.toString();
           return resultat;
        }
    }
    return "Adresse Introuvable";//Le serveur n'est pas dans le même réseau peut-être
}

//************** Procedure de connexion ***************************************************************************

void Connexion::connexion()

{
    cout<<"nouveau client"<<endl;
    sendtoclient("Le client est connecte");
    QTcpSocket *nouveauClient = server->nextPendingConnection();
    client << nouveauClient;
    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(getdata()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexion()));
}

//************** Procedure de récupération du message*******************************************************

void Connexion::getdata()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket==0) //on n'a pas trouvé l'émetteur du message et on quitte
        return;
    QDataStream in(socket);
    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() <(int)sizeof(quint16))
                return;//on n'a pas recu la taille -> on quitte
        in>> tailleMessage; //on récupère la taille reçue
    }
    //on verifie que le message est complet
    if (socket->bytesAvailable() < tailleMessage)
        return; //on n'a pas tout recu on ne peut pas traiter l'info. On quitte
    QString message;
    in>>message;
    sendtoclient(message); //on envoie le message au(x) client en echo (option bien sûr)
    cout<<"message transféré"<<endl;
    tailleMessage=0;     //on remet la variable de taille à 0
}

//************** Procedure de déconnexion du client *********************************************************
void Connexion::deconnexion()
{
    sendtoclient("Un client vient de se deconnecter");
    //On détermine le client (s'il y en a plusieurs)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket==0)   //on n'a pas trouvé le client -> on quitte la procedure
        return;
    client.removeOne(socket);
    socket->deleteLater();
}
//************** Procedure d'envoie de message au client **************************************************

string Connexion::sendtoclient(const QString &message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
       out << (quint16) 0;// 0 au début du paquet pour réserver la place nécessaire pour le nbre de paquets
       out <<message; //on écrit le message
       out.device()->seek(0); //on se remet au debut de paquet
       out <<(quint16)(paquet.size() - sizeof(quint16)); //on remplace le 0
       //on envoie aux clients
       for
               (int i=0; i<client.size();i++)
       {
              client[i]->write(paquet); //on envoie les paquest de données au client
       }
       return "Message sent successfully";
   }

//************************************Slots de redirection des signaux****************************************************

void Connexion::SconnexionNvJoueur(Joueur player)
{
    emit connexionNvJoueur(player);
}

void Connexion::Sattaque(QString from, QString to, uchar posx, uchar posy)
{
    emit attaque(from,to,posx,posy);
}

void Connexion::SplayerGiveUp(QString player)
{
    emit playerGiveUp(player);
}

//**********************************Public Slots***********************************************************************
void Connexion::gameStarted()
{

}

void Connexion::attackResult(QString who, uchar wherex, uchar wherey, bool in_the_water)
{

}

void Connexion::playerLost(QString who)
{

}

void Connexion::playerWon(QString winner)
{

}
void Connexion::tchat(QString message)
{
    this->sendtoclient(message);
}
