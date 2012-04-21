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


Motor::Motor(int motorNum, int numMotors, QObject *parent) : QObject(parent), bus(QDBusConnection::sessionBus()) {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);

    bus.connect("", "/", "edu.vt.ece.msg", "msg", this, SLOT(recvMessage(QString, int)));
    bus.connect("", "/", "edu.vt.ece.updateAngles", "updateAngles", this, SLOT(recvUpdate(double, double, double, double, double, double)));

    QDBusMessage reply = QDBusMessage::createSignal("/", "edu.vt.ece.procStart", "procStart");
    QString foo = QString::number(motorNum);
    foo.append(QString::number(numMotors));
    reply << foo;
    bus.send(reply);

    motorNumber = motorNum;
    motorPosition = ((360/numMotors) * (motorNum)) * (PI/180);

    //initiate PID variables
    //These are tuned by hand with trial and error
    // I wouldn't mess with them unless you REALLY know what you're doing :)
    kp = .5;     //proportional gain (how fast we should get there)
    ki = 0;     //integral gain (error over time)
    kd = .005 ;   //derivative gain

    alt_kp = 10;     //proportional gain (how fast we should get there)
    alt_ki = 0;     //integral gain (error over time)
    alt_kd = .01 ;   //derivative gain

    pitch_integral = 0;
    pitch_prevError = 0;
    roll_integral = 0;
    roll_prevError = 0;
    alt_integral = 0;
    alt_prevError = 0;

    //initiate values
    throttle = 0;
    curPitch = 0;
    curRoll = 0;
    curAltitude = 0;
    dt = 0.01;

    targetPitch = 0;
    targetRoll = 0;
/*
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(motorController()));
    timer->start(dt*1000); //10ms timer
*/
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

void Motor::recvUpdate(double pitch, double roll, double altitude,
                        double tPitch, double tRoll, double tAltitude) {
    this->curPitch = sin(motorPosition)*pitch;
    this->curRoll = cos(motorPosition)*roll;
    this->curAltitude = altitude;

    this->targetPitch = sin(motorPosition)*tPitch;
    this->targetRoll = cos(motorPosition)*tRoll;
    this->targetAltitude = tAltitude;

    //run the motor's PID loop to get new throttle values
    motorController(); //based on these new pitch/rolls
}

void Motor::motorController() {
    double pitch_error = targetPitch-curPitch;
    double roll_error = targetRoll-curRoll;
    double alt_error = targetAltitude-curAltitude;

    double pitch_derivative, roll_derivative, alt_derivative;

    simTime += dt;

    if(abs(pitch_error) > 0.01) {
        pitch_integral += (pitch_error+pitch_prevError)*dt;
        if( pitch_integral > 100) {
            pitch_integral = 100;
        }
        if( pitch_integral < 0 ) {
            pitch_integral = 0;
        }
    }
    pitch_derivative = (pitch_error - pitch_prevError) / dt;

    if(abs(roll_error) > 0.01) {
        roll_integral += (roll_error+roll_prevError)*dt;
        if( roll_integral > 100) {
            roll_integral = 100;
        }
        if( roll_integral < 0 ) {
            roll_integral = 0;
        }
    }
    roll_derivative = (roll_error - roll_prevError) / dt;

    if(abs(alt_error) > 0.01) {
        alt_integral += (alt_error+alt_prevError)*dt;
        if( alt_integral > 100) {
            alt_integral = 100;
        }
        if( alt_integral < 0 ) {
            alt_integral = 0;
        }
    }
    alt_derivative = (alt_error - alt_prevError) / dt;

    if(motorNumber == 1 || motorNumber == 3)
        throttle += (pitch_error*kp + pitch_integral*ki + pitch_derivative*kd);
    else
        throttle += (roll_error*kp + roll_integral*ki + roll_derivative*kd);

    throttle += (alt_error*alt_kp + alt_integral*alt_ki + alt_derivative*alt_kd);

    if( throttle > 100 ) {
        throttle = 100;
    }
    if( throttle < 0 ) {
        throttle = 0;
    }

    qDebug() << "Time:" << simTime << "sec Throttle:" << throttle << "Target Pitch:"
             << targetPitch << "Pitch:" << curPitch << "Target Roll:"
             << targetRoll << "Roll:" << curRoll;

    pitch_prevError = pitch_error;
    roll_prevError = roll_error;

    QDBusMessage update = QDBusMessage::createSignal("/", "edu.vt.ece.updateThrottle", "updateThrottle");
    update << motorNumber << throttle;
    bus.send(update);
}
