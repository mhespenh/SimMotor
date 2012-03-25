/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
    03/23/2012 - Implemented dbus communication correctly
***********************************************************************/

#include "Motor.h"
#include <QSocketNotifier>
#include <QDebug>

Motor::Motor(QObject *parent) : QObject(parent), bus(QDBusConnection::sessionBus()), sharedMem("PRIVATE_SHARED") {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);

    bus.connect("", "/", "edu.vt.ece.msg", "msg", this, SLOT(recvMessage(QString, int)));
    readSharedMem();
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
     qDebug() << theData.t0 << theData.t1 << theData.t2 << theData.t3 << "\n";
     return true;
}
