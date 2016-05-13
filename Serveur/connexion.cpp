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
    cout<<"initialisation du serveur..."<<endl;
    //------- Gestion du serveur ------------------------------------------------------
    server=new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 40110)) //erreur lors du démarrage du serveur
        cout<<"Le serveur n'a pas pu être démarré :"<<server->errorString().toStdString()<<endl;
    else{ //le serveur a réussi à s'initialiser
        cout<<"Le serveur demarre:"<<endl<< "\nAdresses:"<<getIPaddress().toStdString()<<endl<<"\t\nPort: "<<server->serverPort()<<endl;
            //on autorise la connexion d'un client. S'il ne doit y avoir qu'un seul client il faudra modifier cette ligne
            connect(server, SIGNAL(newConnection()),this, SLOT(connexion()));
    }
    tailleMessage = 0;//comme il n'y a pas encore eu de message la taille du message est nulle
}

//*************** Rend les adresse IP *******************************************************************************
QString Connexion::getIPaddress(){
    QString resultat;
    foreach(QHostAddress address, QNetworkInterface::allAddresses()){
        resultat.append("\n\t[").append(address.toString()).append("]");
    }
    return resultat;//Le serveur n'est pas dans le même réseau peut-être
}

//************** Procedure de connexion ***************************************************************************
void Connexion::connexion(){
    cout<<"Nouvelle connexion detectée"<<endl;
    sendtoclient("Le client est connecte");
    QTcpSocket *nouveauClient = server->nextPendingConnection();
    client << nouveauClient;
    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(getdata()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexion()));
}

//************** Procedure de récupération du message*******************************************************
void Connexion::getdata(){
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket==0) //on n'a pas trouvé l'émetteur du message et on quitte
        return;
    QDataStream in(socket);
    if (tailleMessage == 0){
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
    socket->readAll();
    tailleMessage=0;     //on remet la variable de taille à 0
}

//************** Procedure de déconnexion du client *********************************************************
void Connexion::deconnexion(){
    sendtoclient("Un client vient de se deconnecter");
    //On détermine le client (s'il y en a plusieurs)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket==0)   //on n'a pas trouvé le client -> on quitte la procedure
        return;
    client.removeOne(socket);
    clients.values().removeOne(socket);
    socket->deleteLater();
}
//************** Procedure d'envoie de message au(x) client(s) **************************************************
void Connexion::sendtoclient(const QString message){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out << (quint16) 0;// 0 au début du paquet pour réserver la place nécessaire pour le nbre de paquets
    out <<message; //on écrit le message
    out.device()->seek(0); //on se remet au debut de paquet
    out <<(quint16)(paquet.size() - sizeof(quint16)); //on remplace le 0
    for(int i=0; i<client.size();i++){
           client[i]->write(paquet);     //On envoie à tous les clients
           client[i]->flush();
    }
}
void Connexion::sendToOneClient(QString message, int whichClient){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out << (quint16) 0;// 0 au début du paquet pour réserver la place nécessaire pour le nbre de paquets
    out <<message; //on écrit le message
    out.device()->seek(0); //on se remet au debut de paquet
    out <<(quint16)(paquet.size() - sizeof(quint16)); //on remplace le 0
    //on envoie aux clients
    client[whichClient]->write(paquet); //on envoie les paquest de données au client
    client[whichClient]->flush();
}

void Connexion::sendToOneClient(QString message, QString name){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out<<(quint16)0;
    out<<message;
    out.device()->seek(0);
    out<<(quint16)(paquet.size() - sizeof(quint16));

    clients[name]->write(paquet);
    clients[name]->flush();
}

void Connexion::kick(QString name)
{
    if(!clients.keys().contains(name))
        return;
    QTcpSocket * socket = clients[name];
    socket->close();
    client.removeOne(socket);
    clients.values().removeOne(socket);
    socket->deleteLater();
}

void Connexion::kickClean()
{
    QList<QTcpSocket *> players = clients.values();

    for(int i = 0;i<client.size();i++){
        if(!players.contains(client[i])){
            QTcpSocket * socket = client[i];
            socket->close();
            client.removeOne(socket);
            socket->deleteLater();
        }
    }

}

//**********************************Public Slots***********************************************************************
void Connexion::gameStarted(){
    this->sendtoclient(Header::GameStarted);
}

void Connexion::attackResult(QString who, uchar wherex, uchar wherey, bool in_the_water){
    this->sendtoclient(QString(Header::PlayerAttack)+":"+who+":"+QString::number(wherex)+":"+QString::number(wherey)+":"+QString::number(in_the_water));
}

void Connexion::playerLost(QString who){
    this->sendtoclient(QString(Header::PlayerLost)+":"+who);
}

void Connexion::playerWon(QString winner){
    this->sendtoclient(QString(Header::PlayerWin)+":"+winner);
}
void Connexion::tchat(QString message){
    if(message[0]!=Header::Message)
        message.prepend(Header::Message);
    cout<<message.toStdString()<<endl;
    this->sendtoclient(message);
}

void Connexion::messageGestion(QString message){
    //cout <<"Reception d'un message : "<< message.toStdString()<<endl;
    if(message[0]==Header::Message){
        emit tchat(message);
    }
    else if(message[0]== Header::NewPlayer){
        Bateau *boats;
        cout << message.toStdString() << endl;
        message=message.remove(0,1);
        QStringList messageSplit=message.split(":");
        QString Name=messageSplit.at(0);
        message=message.remove(0,Name.capacity()+1);
        QStringList positions=message.split(":");
        cout <<Name.toStdString()+"a envoyé ses bateaux"<<endl;
        boats = new Bateau[Joueur::DEFAULT_BOAT_NUMBER];
        cout<< "Positionnement :"<<endl;
        for (int i=0 ; i<Joueur::DEFAULT_BOAT_NUMBER ; i++){
            QString taille=positions.at(4*i+2);
            QString posX=positions.at(4*i);
            QString PosY=positions.at(4*i+1);
            QString IsHorizontal=positions.at(4*i+3);
            cout<<"Taille : "<<taille.toStdString()<<"\tPosX : "<<posX.toStdString()<<"PosY : "<<PosY.toStdString()<<"IsHorizontal : "<<IsHorizontal.toStdString()<<endl;
            boats[i]= Bateau(taille.toInt(),posX.toInt(),PosY.toInt(), IsHorizontal.toInt());
        }
       Joueur player = Joueur(boats, Name, 8);
       clients.insert(Name,qobject_cast<QTcpSocket *>(sender()));
       emit connexionNvJoueur(player);
    }
    else if (message[0]==Header::GiveUp){
        QString playerName;
        playerName=message.remove(0,1);
        emit playerGiveUp(playerName);
    }
    else if (message[0]==Header::PlayerAttack){
        QStringList lst = message.split(":");
        emit attaque(lst[1],lst[2],(unsigned char)lst[3].toInt(),(unsigned char)lst[4].toInt());
    }
    else if(message[0]==Header::NewName){
        message=message.remove(0,1);
        names.append("");
        for (int i=0; i<names.size();i++){
            if(names[i]==message){
                sendToOneClient(Header::NewNameError, client.size()-1);
                cout<<"Un client a essayé de se connecter avec un nom déjà utilisé"<<endl;
                break;
            }
            else if(names[i]==""){
                names[i]=message;
                sendtoclient(QString(Header::NewName)+":"+names.join(":"));
                cout<<"Envoit de la liste des noms"<<endl;
                break;
            }
        }
    }
}
