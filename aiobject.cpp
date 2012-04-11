#include "aiobject.h"

AIObject::AIObject(int start_x, int start_y, int fin_x, int fin_y, int arm_length)
{
    arm_len = arm_length;
    dest_x = fin_x;
    dest_y = fin_y;
    cur_x = start_x;
    cur_y = start_y;
    pop_environment();
    scan_results = scan(cur_x, cur_y);
}

void AIObject::pop_environment()
{
    //Populate environment 1 for occupied, 0 for unoccupied
    for(int i = 0; i < ENVSIZE; i++){
        for(int y = 0; y < ENVSIZE; y++){
            if(i >= 100 && i <= 110 && y >=100 && y <= 110){
                environment[i][y] = 1;
            }
            else environment[i][y] = 0;
        }
    }
}

QList<bool> AIObject::scan(int x, int y)
{
    QList<bool> retValue;
    bool N = false, E = false, S = false, W = false;
    int new_arm = ceil(float(arm_len)/10.0);
    int x_val, y_val;
    
    // N, E, S, W -- (x,y) locations of proximity sensors
    int x_array[4] = {x, (x+new_arm), x, (x-new_arm)};
    int y_array[4] = {(y+new_arm), y, (y-new_arm), y};

    retValue << N << E << S << W;
    return retValue;
}
