#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

class Command
{
public:
    static void doCommand(QString command, QString* args);
};

#endif // COMMAND_H
