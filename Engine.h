/***********************************************************************
  Group 3 ECE4574 Spring 2012
  Project 3 - Multicopter Simulator
    SimEngine - A simulatable engine module (master branch)

  Revisions:
    03/21/2012 - Initial version (master branch)
***********************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);

signals:

private slots:
    void onData(void);
};

#endif // ENGINE_H
