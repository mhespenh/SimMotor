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

#include "Motor.h"
#include <QSocketNotifier>
#include <QDebug>

#define PI 3.14159265

Motor::Motor(int motorNum ,QObject *parent) : QObject(parent), bus(QDBusConnection::sessionBus()) {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);

    bus.connect("", "/", "edu.vt.ece.msg", "msg", this, SLOT(recvMessage(QString, int)));
    bus.connect("", "/", "edu.vt.ece.updateAngles", "updateAngles", this, SLOT(recvUpdate(double, double, double)));

    QDBusMessage reply = QDBusMessage::createSignal("/", "edu.vt.ece.procStart", "procStart");
    QString foo = QString::number(motorNum);
    reply << foo;
    bus.send(reply);

    kp = 5;     //proportional gain
    ki = 3;   //integral gain
    kd = 0.05;  //derivative gain
    motorNumber = motorNum;
    throttle = 0;
    integral = 0;
    prevError = 0;
    simTime = 0;
    pitch = 0;
    roll = 0;
    setPoint = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(motorController()));
    timer->start(dt*1000); //10ms timer
}

Motor::~Motor() {
}

void Motor::onData() {
    //Connect a QTextStream to the stdin as readonly
    QTextStream stream(stdin, QIODevice::ReadOnly);
    QString str;
    str += stream.readLine(); //read from the stdin
    qDebug() << "Received: " << str << "\nSent from Motor Process";
}

void Motor::recvMessage(QString msg, int type) {
    qDebug() << "Received Message: " << msg << "of type" << type;
}

void Motor::recvUpdate(double newpitch, double newroll, double newaltitude) {
    this->pitch = newpitch;
    this->roll = newroll;
    this->altitude = newaltitude;
}

void Motor::motorController() {
    if( pitch > setPoint-1) {
        QCoreApplication::exit(0);
    }
    double error = setPoint-pitch;
    double derivative;
    qDebug() << "Time:" << simTime << "sec Throttle:" << throttle << "Set point:"
             << setPoint << "Pitch:" << pitch+1;
    simTime += dt;
    if(abs(error) > 0.01) {
        integral += (error+prevError)*dt;
        if( integral > 100) {
            integral = 100;
        }
        if( integral < 0 ) {
            integral = 0;
        }
    }
    derivative = (error - prevError) / dt;
    throttle = error*kp + integral*ki + derivative*kd;

    if( throttle > 100 ) {
        throttle = 100;
    }
    if( throttle < 0 ) {
        throttle = 0.001;
    }

    prevError = error;
    QDBusMessage update = QDBusMessage::createSignal("/", "edu.vt.ece.updateThrottle", "updateThrottle");
    update << "Motor " << motorNumber << " throttle " << throttle;
    bus.send(update);
}
