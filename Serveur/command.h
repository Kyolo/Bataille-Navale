#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>
#include <game.h>

class Command
{
public:
    static void doCommand(QString command, QStringList args);
};

#endif // COMMAND_H
