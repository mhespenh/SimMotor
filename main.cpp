/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimEngine - A simulatable engine module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
***********************************************************************/

#include <QtCore/QCoreApplication>
#include <Engine.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Engine theEngine; //instantiate an engine object
    return a.exec();
}
