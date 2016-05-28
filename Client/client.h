#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>

typedef unsigned char uchar;

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
    void rename();
    void NewNameSignal(QString);
    void AttackReceived(QString,uchar,uchar,bool);
    void signalPlayerLost(QString);
    void signalPlayerWin(QString);
    void gmeStart();
    void newAdversaire(QString);
    void GestTours();
};

#endif // CLIENT_H
