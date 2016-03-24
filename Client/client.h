#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <string>

 void comthread(QString message);

class client: public QObject
{

    Q_OBJECT
public:
    client(std::string addr, quint16 port);
    void send(QString DonneesAEnvoyer);

private:
    void connexion(std::string addr, quint16 port);
    QTcpSocket *socket;
    quint16 tailleMessage;
    void msgGestion(QString message);
private slots:
    void connecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void deconnecte();
    void getserverdata();
signals:
    void tchatRecive(QString);
    void serverError(QString);
};

#endif // CLIENT_H
