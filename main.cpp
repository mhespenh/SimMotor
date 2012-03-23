/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimMotor - A simulatable Motor module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
    03/23/2012 - Implemented dbus communication
***********************************************************************/

#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <Motor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                        "To start it, run:\n"
                        "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService("edu.vt.ece.simmotor")) {
        fprintf(stderr, "%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    Motor theMotor; //instantiate a Motor object
    QDBusConnection::sessionBus().registerObject("/", &theMotor, QDBusConnection::ExportAllSlots);

    return a.exec();
}
