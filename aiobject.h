#ifndef AIOBJECT_H
#define AIOBJECT_H

#define SCANDIST 15
#define ENVSIZE 600

#include<QDebug>
#include<QList>
#include <math.h>

class AIObject
{
public:
    AIObject(int, int, int, int, int);
private:
    void pop_environment();
    QList<bool> scan(int, int);

    QList<bool> scan_results;
    int environment[600][600], cur_x, cur_y, prev_x, prev_y, dest_x, dest_y, arm_len;
};

#endif // AIOBJECT_H
