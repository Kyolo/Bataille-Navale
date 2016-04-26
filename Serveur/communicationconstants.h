#ifndef COMMUNICATIONCONSTANTS_H
#define COMMUNICATIONCONSTANTS_H

#include <QString>


namespace Header {
const static int Message=0x01;
const static int GiveUp=0x02;
const static int NewPlayer=0x03;
const static int PlayerAttack=0x04;
const static int PlayerLost=0x05;
const static int PlayerWin=0x06;
const static int GameStarted=0x07;
const static int GameFinished=0x08;
const static int NewName=0x09;
QString NewNameError = "a";
}

#endif // COMMUNICATIONCONSTANTS_H
