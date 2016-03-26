#include "messagegestion.h"

#include <QString>

#include "communicationconstants.h"


using namespace std;

messageGestion::messageGestion()
{

}

void messageGestion::inputMessage(QString message)
{
    if(message[0]==MessageHeader)
    {
        emit tchat(message);
    }
    else if (message[0]==GiveUpHeader)
    {
        QString playerName;
        playerName=message.remove(0,1);
        cout << "le joueur"<< playerName.toStdString() <<"a quitte le jeu"<<endl;
        emit playerGiveUp(playerName);
    }
}
