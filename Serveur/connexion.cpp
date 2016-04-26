#include "connexion.h"
#include "communicationconstants.h"
#include "bateau.h"
#include "joueur.h"


#include <QTcpServer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QDataStream>

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <QString>

using namespace std;
using namespace Header;

Connexion::Connexion()
{
    cout<<"Initialisation du serveur..."<<endl;
    /*for (int i=0; i<10;i++)
    {
        names[i]="";
     } *///Après vérification, c'est inutile
    //------- Gestion du serveur ------------------------------------------------------
        server=new QTcpServer(this);
        if (!server->listen(QHostAddress::Any, 40110)) //utilisation du port 40110 pour le serveur
        { //erreur lors du démarrage du serveur
            cout<<"Le serveur n'a pas pu être démarré : "<<
                   server->errorString().toStdString()<<endl;
        }
        else
        { //le serveur a réussi à s'initialiser
            cout<<"Le serveur est en attente de connexion"<<endl<< "Adresse locale : "<<getIPaddress().toStdString()<<endl<<
                   "port : "<<server->serverPort()<<endl;
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
    cout<<"Nouvelle connexion d'un client détectée"<<endl;
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
    messageGestion(message);//on renvoie le message vers la gestion des messages
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

void Connexion::sendtoclient(const QString &message)
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
   }
<<<<<<< HEAD
string Connexion::sendToOneClient( QString &message, int witchClient)
=======
void Connexion::sendToOneClient(const QString &message, int whichClient)
>>>>>>> c1b8438cd5314eeea473007573e57bf20b7223d1
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
       out << (quint16) 0;// 0 au début du paquet pour réserver la place nécessaire pour le nbre de paquets
       out <<message; //on écrit le message
       out.device()->seek(0); //on se remet au debut de paquet
       out <<(quint16)(paquet.size() - sizeof(quint16)); //on remplace le 0
       //on envoie aux clients
       client[whichClient]->write(paquet); //on envoie les paquets de données au client
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

void Connexion::messageGestion(QString message)
{
    cout <<"Réception d'un message..."<<endl<<"\t" <<message.toStdString()<<endl;
    if(message[0]==Header::Message)
    {
        emit tchat(message);
    }
    else if(message[0]== NewPlayer)
    {
        message=message.remove(0,1);
        QStringList messageSplit=message.split(":");
        QString Name=messageSplit.at(0);
        message=message.remove(0,Name.capacity()+1);
        QStringList positions=message.split(":");
        cout <<Name.toStdString()<<endl;
        for (int i=0 ; i<8 ; i++)
        {
            QString taille=positions.at(4*i+2);
            QString posX=positions.at(4*i);
            QString PosY=positions.at(4*i+1);
            QString IsHorizontal=positions.at(4*i+3);
            boats[i]= new Bateau(taille.toInt(),posX.toInt(),PosY.toInt(), IsHorizontal.toInt());
        }
        player = new Joueur(*boats, Name, 8);
        emit connexionNvJoueur(*player);
    }
    else if (message[0]==Header::GiveUp)
    {
        QString playerName;
        playerName=message.remove(0,1);
        emit playerGiveUp(playerName);
    }
    else if (message[0]==Header::PlayerAttack){
        QStringList lst = message.split(":");
        emit attaque(lst[1],lst[2],(unsigned char)lst[3].toInt(),(unsigned char)lst[4].toInt());
    }
    else if(message[0]==NewName)
    {
        message=message.remove(0,1);
        for (int i=0; i<10;i++)
        {
            if(names[i]==message)
            {
<<<<<<< HEAD
                sendToOneClient(NewNameError, client.size()-1);
                cout<<"nomIdentique"<<endl;
=======
                sendToOneClient(QString::number(NewNameError), client.size()-1);
                cerr<<"Erreur : "<<message.toStdString()<<" est un nom déjà prit"<<endl;
>>>>>>> c1b8438cd5314eeea473007573e57bf20b7223d1
                break;
            }
            else if(names[i]=="")
            {
                names[i]=message;
                QString messageNames="";
                for (int a=0; a<=i;a++)
                {
                    cout <<"plantage imminent"<<endl;
                    messageNames=messageNames+names[a]+":";
                }
                sendToOneClient(messageNames,client.size()-1);
                break;
            }
        }
    }
}
