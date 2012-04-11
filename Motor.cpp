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

// Test push

#include "Motor.h"
#include <QSocketNotifier>
#include <QDebug>

#define PI 3.14159265

Motor::Motor(QObject *parent) : QObject(parent), bus(QDBusConnection::sessionBus()), sharedMem("PRIVATE_SHARED") {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);

    bus.connect("", "/", "edu.vt.ece.msg", "msg", this, SLOT(recvMessage(QString, int)));
    QDBusMessage reply = QDBusMessage::createSignal("/", "edu.vt.ece.procStart", "procStart");
    reply << "Started OKAY";
    bus.send(reply);
    readSharedMem();

    kp = 5;     //proportional gain
    ki = 3;   //integral gain
    kd = 0.05;  //derivative gain
    dt = 0.01;     //simulation time
    
    throttle = 0;
    integral = 0;
    prevError = 0;
    simTime = 0;
    pitch = 0;
    roll = 0;
    setPoint = 30;

    QTimer *physicsTimer = new QTimer(this);
    connect(physicsTimer, SIGNAL(timeout()), this, SLOT(updatePhysics()));
    physicsTimer->start(dt*1000); //10ms timer

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

    QDBusMessage reply = QDBusMessage::createSignal("/", "edu.vt.ece.ack", "ack");
    reply << "Received OKAY";
    bus.send(reply);
    QCoreApplication::exit(4);
}

bool Motor::readSharedMem() {
    if (!sharedMem.attach()) {
         return false;
    }
     data theData;
     data* ptr;

     sharedMem.lock();
     ptr = (data*)sharedMem.data();
     theData.t0 = ptr->t0;
     theData.t1 = ptr->t1;
     theData.t2 = ptr->t2;
     theData.t3 = ptr->t3;
     sharedMem.unlock();

     sharedMem.detach();
     qDebug() << "Shared Memory contains: " << theData.t0 << theData.t1 << theData.t2 << theData.t3;
     return true;
}

void Motor::motorController() {
    if( pitch > setPoint-1) {
        QCoreApplication::exit(0);
    }
    float error = setPoint-pitch;
    float derivative;
    qDebug() << "Time:" << simTime << "sec Throttle:" << throttle << "Set point:"
             << setPoint << "Pitch:" << pitch;
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
}

void Motor::updatePhysics() {
    pitch += ((throttle-25)*cos(pitch*(PI/180)))*cos(roll*(PI/180)) * 0.1;
}
