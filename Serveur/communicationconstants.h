#ifndef COMMUNICATIONCONSTANTS_H
#define COMMUNICATIONCONSTANTS_H

namespace Header {
const static int Message=0x01;
const static int GiveUp=0x02;
const static int NewPlayer=0x03;
const static int PlayerAttack=0x04;
const static int PlayerLost=0x05;
const static int PlayerWin=0x06;
const static int GameStarted=0x07;
const static int GameFinished=0x08;
}

#endif // COMMUNICATIONCONSTANTS_H
