#include <QtCore/QCoreApplication>
#include <Engine.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Engine theEngine;
    return a.exec();
}
