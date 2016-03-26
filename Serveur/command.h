#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>
#include <QThread>

#include <connexion.h>

class CommandManager: public QThread
{
public:
    CommandManager();
    static void activateCommand(QString command, QStringList args);

private:
    void run();
    bool continuer;

};

#endif // COMMAND_H
