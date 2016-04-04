#include "messagegestion.h"

#include <QString>
#include <QStringList>

#include "communicationconstants.h"


using namespace std;

messageGestion::messageGestion()
{

}

void messageGestion::inputMessage(QString message)
{
    if(message[0]==Header::Message)
    {
        emit tchat(message);
    }
    else if (message[0]==Header::GiveUp)
    {
        QString playerName;
        playerName=message.remove(0,1);
        emit splayerGiveUp(playerName);
    } else if (message[0]==Header::PlayerAttack){
        QStringList lst = message.split(":");
        emit attaque(lst[1],lst[2],(unsigned char)lst[3].toInt(),(unsigned char)lst[4].toInt());
    }
}