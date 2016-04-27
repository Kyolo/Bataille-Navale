#ifndef COMUNICATIONCONSTANTS_H
#define COMUNICATIONCONSTANTS_H

namespace Header {
    const int MessageHeader = 0x01;
    const int GiveUpHeader = 0x02;
    const int NewPlayer = 0x03;
    const int PlayerAttack = 0x04;
    const int PlayerLost = 0x05;
    const int PlayerWin = 0x06;
    const int GameStarted = 0x07;
    const int GameFinished = 0x08;
    const int NewName = 0x09;
    const int NewNameError = 0x0A;
}

#endif // COMUNICATIONCONSTANTS_H
