/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
    03/23/2012 - Implemented dbus communication correctly
***********************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QSharedMemory>

struct data {
    int t0, t1, t2, t3;
};

class Motor : public QObject
{
    Q_OBJECT
public:
    explicit Motor(QObject *parent = 0);

public slots:
    Q_SCRIPTABLE void recvMessage(QString, int);

signals:

private slots:
    void onData(void);
private:
    bool readSharedMem();
    QDBusConnection bus;
    QSharedMemory sharedMem;
};

#endif // MOTOR_H
