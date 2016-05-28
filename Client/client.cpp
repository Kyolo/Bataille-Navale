#include "client.h"
#include <iostream>
#include <stdio.h>

#include "comunicationconstants.h"

using namespace std; using namespace Header;

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
{
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
           emit serverError("ERREUR : le serveur n'a pas pu etre trouvé. Verifiez l'IP et le port.");
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
       QString message="";
       in >> message;
       this->msgGestion(message);
       socket->readAll();
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
    socket->flush();
}

void client::msgGestion(QString message)
{
    cout<<"nouveau message"<<endl;
    cout<<message.toStdString()<<endl;

    if(message[0]==MessageHeader){
        message.remove(0,1);
        emit tchatRecive(message);
    }else if(message[0]==NewNameError){
        cout<<"Votre nom est déjà utilisé"<<endl;
        emit rename();
    }else if(message[0]==NewName){
        cout<<"Reception des noms des autres joueurs"<<endl;
        message.remove(0,1);
        QStringList nameList = message.split(":");
        for(int i=0;i<nameList.size();i++){
            if(((QString)nameList.at(i)).isEmpty())
                continue;//Si jamais le nom est vide, on passe au suivant
            emit NewNameSignal(nameList.at(i));
        }
    }else if(message[0]==PlayerAttack){
        QStringList par = message.split(":");
        emit AttackReceived(par[1],(uchar)((QString)par[2]).toInt(),(uchar)((QString)par[3]).toInt(),(bool)((QString)par[4]).toInt());
    } else if(message[0]==PlayerLost){
        emit signalPlayerLost((QString)(message.split(":").at(1)));
    }else if(message[0]==PlayerWin){
        emit signalPlayerWin((QString)(message.split(":").at(1)));
    }else if(message[0]==GameStarted){
        cout<<"game started"<<endl;
        emit gmeStart();
    } else if(message[0]==NewPlayer){
        emit newAdversaire((QString)(message.split(":").at(1)));
    }else if(message[0]==NewRound){
       emit GestTours();
    }
}
