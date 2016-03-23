#include "client.h"
#include <iostream>
#include <stdio.h>

using namespace std;

client::client()
{
    socket = new QTcpSocket(this); //créé l'instance de QTcpSocket
    inth = new ThreadCin();
    inth->start();

         connect(socket, SIGNAL(readyRead()), this, SLOT(getserverdata()));//gestion de la donnée serveur
         connect(socket, SIGNAL(connected()), this, SLOT(connecte())); //gestion de signal de connexion
         connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte())); //gestion de signal de déconnexion
         connect(inth, SIGNAL(entreeClavier()), this, SLOT(cinMessage()));
         connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(erreurSocket(QAbstractSocket::SocketError)));//gestion de signal d'erreur
         tailleMessage = 0;
         this->connexion();

}

void client::connecte()
{ //lorsque le signal de connexion est reçu du serveur le client affiche un message et les paramétrages
   //ne peuvent plus être modifiés
    cout<<"Connexion reussie !"<<endl;
}

void client::connexion()
{//l'utilisateur a cliqué sur le bouton de connexion
    string cinAddr;
    quint16 port;
    cout<<"Connexion au serveur"<<endl<<"Adresse IP :";
    cin>> cinAddr;
    cout<<"Numero de port :";
    cin>>port;
    cout<<"Tentative de connexion ! "<<endl; //on ecrit ce qui se passe
    socket->abort(); //on désactive les connexions précédentes
    QHostAddress IpAdress(QString::fromStdString(cinAddr));
    socket->connectToHost(IpAdress, port);
}

void client::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
           cout<<"ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port."<<endl;
            break;
        case QAbstractSocket::ConnectionRefusedError:
            cout<<"ERREUR : le serveur a refuse la connexion. Verifiez si le programme \"serveur\" a bien ete lance. Verifiez aussi l'IP et le port."<<endl;
            break;
        case QAbstractSocket::RemoteHostClosedError:
           cout<<"ERREUR : le serveur a coupé la connexion."<<endl;
            break;
        default:
            cout<<"ERREUR : "<< socket->errorString().toStdString()<<endl;
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
       // On affiche le message chez le client
       cout<<message.toStdString()<<endl;
       // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
       tailleMessage = 0;
}

void client::send(QString DonneesAEnvoyer)
{//l'utilisateur a cliqué sur le bouton envoyer au serveur
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out<<(quint16) 0; //on réserve l'entete pour la taille des données
    out<<DonneesAEnvoyer;//on ajoute la donnée
    out.device()->seek(0);//on retourne au début
    out<<(quint16)(paquet.size() - sizeof(quint16));
    socket->write(paquet); //on écrit la donnée sur le socket
}

void client::cinMessage()
{
    string message;
    while(kbhit())
    {
        getch();
    }
    cout<<"message :";
    getline(cin,message);
    send(QString::fromStdString(message));
    while(kbhit())
    {
        getch();
    }
}
