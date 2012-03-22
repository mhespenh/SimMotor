/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
***********************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

class Motor : public QObject
{
    Q_OBJECT
public:
    explicit Motor(QObject *parent = 0);

signals:

private slots:
    void onData(void);
};

#endif // MOTOR_H
