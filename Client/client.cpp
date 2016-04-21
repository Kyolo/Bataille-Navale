#include "client.h"
#include <iostream>
#include <stdio.h>

#include "comunicationconstants.h"

using namespace std;

client::client(string addr, quint16 port)
{
    socket = new QTcpSocket(this); //créé l'instance de QTcpSocket

         connect(socket, SIGNAL(readyRead()), this, SLOT(getserverdata()));//gestion de la donnée serveur
         connect(socket, SIGNAL(connected()), this, SLOT(connecte())); //gestion de signal de connexion
         connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte())); //gestion de signal de déconnexion
         connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(erreurSocket(QAbstractSocket::SocketError)));//gestion de signal d'erreur
         tailleMessage = 0;
         this->connexion(addr, port);

}

void client::connecte()
{ //lorsque le signal de connexion est reçu du serveur le client affiche un message et les paramétrages
   //ne peuvent plus être modifiés
    cout<<"Connexion reussie !"<<endl;
}

void client::connexion(string addr, quint16 port)
{//l'utilisateur a cliqué sur le bouton de connexion
    socket->abort(); //on désactive les connexions précédentes
    QHostAddress IpAdress(QString::fromStdString(addr));
    socket->connectToHost(IpAdress, port);
}

void client::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
           emit serverError("ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            emit serverError("ERREUR : le serveur a refuse la connexion. Verifiez si le programme \"serveur\" a bien ete lance. Verifiez aussi l'IP et le port.");
            break;
        case QAbstractSocket::RemoteHostClosedError:
           emit serverError("ERREUR : le serveur a coupé la connexion.");
            break;
        default:
            emit serverError("ERREUR : "+ socket->errorString());
    }
}

void client::deconnecte()
{
    cout<<"Déconnecté du serveur"<<endl;
}

void client::getserverdata()
{ // Affiche les données reçues par le serveur
   QDataStream in(socket);
   if (tailleMessage == 0)
       {//taille non recupérée
           if (socket->bytesAvailable() < (int)sizeof(quint16))
                return;
           in >> tailleMessage;
       }
       if (socket->bytesAvailable() < tailleMessage)
           return;
       // On a toutes les infos nécessaires pour le message
       QString message;
       in >> message;
       this->msgGestion(message);
       tailleMessage = 0;
}

void client::send(QString DonneesAEnvoyer)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out<<(quint16) 0; //on réserve l'entete pour la taille des données
    out<<DonneesAEnvoyer;//on ajoute la donnée
    out.device()->seek(0);//on retourne au début
    out<<(quint16)(paquet.size() - sizeof(quint16));
    socket->write(paquet); //on écrit la donnée sur le socket
}

void client::msgGestion(QString message)
{
    if(message[0]==MessageHeader)
    {
        message.remove(0,1);
        emit tchatRecive(message);
    }
}
