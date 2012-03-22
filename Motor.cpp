/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
***********************************************************************/

#include "Motor.h"
#include <QSocketNotifier>
#include <QDebug>

Motor::Motor(QObject *parent) : QObject(parent) {
    QSocketNotifier *inNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    QObject::connect(inNotifier, SIGNAL(activated(int)), this, SLOT(onData()));
    inNotifier->setEnabled(true);
}

void Motor::onData() {
    QTextStream stream(stdin, QIODevice::ReadOnly);
    QString str;
    while(1) {
        fd_set stdinfd;
        FD_ZERO( &stdinfd );
        FD_SET( STDIN_FILENO, &stdinfd );
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int ready = select( 1, &stdinfd, NULL, NULL, &tv );
        if( ready > 0 ) {
                str += stream.readLine();
        }
        else {
                break;
        }
    }
    qDebug() << "Received: " << str << "\nSent from Motor Process";
}
