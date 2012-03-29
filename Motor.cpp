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

Motor::Motor(QObject *parent) : QObject(parent), bus(QDBusConnection::sessionBus()), sharedMem("PRIVATE_SHARED") {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);

    bus.connect("", "/", "edu.vt.ece.msg", "msg", this, SLOT(recvMessage(QString, int)));
    QDBusMessage reply = QDBusMessage::createSignal("/", "edu.vt.ece.procStart", "procStart");
    reply << "Started OKAY";
    bus.send(reply);
    readSharedMem();

    kp = 4;     //proportional gain
    ki = 0.3;   //integral gain
    kd = 0.01;  //derivative gain
    dt = 0;     //simulation time
    
    throttle = 0;
    integral = 0;
    prevError = 0;
    angle = 5;
    setPoint = 25;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(motorController()));
    timer->start(10); //10ms timer
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
    float error = setPoint-angle;
    float derivative;
    qDebug() << "Time: " << dt << "sec Throttle" << throttle << " Set point: "
             << setPoint << " Angle:" << angle;
    dt += 0.01;

    if(abs(error) > 0.1) {
        integral = integral + ((error+prevError)*ki);
        if( integral > 100) {
            integral = 100;
        }
        if( integral < 0 ) {
            integral = 0;
        }
    }
    derivative = (error - prevError)*kd;
    throttle = kp*error + integral + derivative;

    if( throttle > 100 ) {
        throttle = 100;
    }
    if( throttle < 0 ) {
        throttle = 0.001;
    }

    prevError = error;

    angle += ((throttle-25)*cos(angle*(PI/180))) * 0.1;
}
