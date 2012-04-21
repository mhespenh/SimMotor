/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
    03/23/2012 - Implemented dbus communication correctly
    03/25/2012 - Implemented a rough example of QSharedMemory
***********************************************************************/

#include <QtCore/QCoreApplication>
#include <stdio.h>
#include <Motor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int motorNum = atoi(argv[1]);
    int numMotors = atoi(argv[2]);
    Motor theMotor(motorNum, numMotors); //instantiate a Motor object

    return a.exec();
}
