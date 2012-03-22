/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
***********************************************************************/

#include <QtCore/QCoreApplication>
#include <Motor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Motor theMotor; //instantiate a Motor object
    return a.exec();
}
