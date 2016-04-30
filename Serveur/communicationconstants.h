#ifndef COMMUNICATIONCONSTANTS_H
#define COMMUNICATIONCONSTANTS_H

#include <QChar>


namespace Header {
const static QChar Message='a';
const static QChar GiveUp='b';
const static QChar NewPlayer='c';
const static QChar PlayerAttack='d';
const static QChar PlayerLost='e';
const static QChar PlayerWin='f';
const static QChar GameStarted='g';
const static QChar GameFinished='h';
const static QChar NewName='i';
const static QChar NewNameError='j';
}

#endif // COMMUNICATIONCONSTANTS_H
