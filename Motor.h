/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
    03/23/2012 - Implemented dbus communication correctly
    03/25/2012 - Implemented a rough example of QSharedMemory
    03/28/2012 - Implemented a basic PID controller
***********************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QSharedMemory>


#define PI 3.1415926535897932384626433832795028841971693993751058209


struct data {
    int t0, t1, t2, t3;
};

class Motor : public QObject
{
    Q_OBJECT
public:
    explicit Motor(int, int, QObject *parent = 0);
    ~Motor();

public slots:
    Q_SCRIPTABLE void recvMessage(QString, int);
    Q_SCRIPTABLE void recvUpdate(double, double, double, double, double, double);

private slots:
    void motorController(void);

signals:

private slots:
    void onData(void);

private:
    bool readSharedMem();
    QDBusConnection bus;
    QSharedMemory sharedMem;
    double kp, ki, kd, dt, simTime;
    double alt_kp, alt_kd,alt_ki;
    double throttle, pitch_prevError, pitch_integral;
    double roll_prevError, roll_integral;
    double alt_prevError, alt_integral;
    double curPitch, curRoll, curAltitude;
    double targetPitch, targetRoll, targetAltitude;
    int motorNumber;
    double motorPosition;
};

#endif // MOTOR_H
