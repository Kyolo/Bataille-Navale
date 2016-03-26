#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>
#include <QThread>

#include <connexion.h>

class CommandManager: public QThread
{
    Q_OBJECT
public:
    CommandManager();

public slots:
    void useCommand(QString command, QStringList args);

private:
    void run();
signals:
    void commandDetected(QString,QStringList);
};

#endif // COMMAND_H
