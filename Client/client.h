#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <string>
#include <Threadinput.h>
#include <conio.h>

 void comthread(QString message);

class client: public QObject
{

    Q_OBJECT
public:
    client();
    void send(QString DonneesAEnvoyer);

private:
    void connexion();
    QTcpSocket *socket;
    ThreadCin *inth;
    quint16 tailleMessage;
private slots:
    void connecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void deconnecte();
    void getserverdata();
    void cinMessage();
};

#endif // CLIENT_H
