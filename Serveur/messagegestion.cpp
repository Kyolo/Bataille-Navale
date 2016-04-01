#include "messagegestion.h"

#include <QString>

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
    }
}
