#ifndef __CUR_PID__
#define __CUR_PID__

#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;

class cur_pid
{
public:
    float k1 = 0;
    float k2 = 0;
    float k3 = 0;

    float error = 0;
    float error_l = 0;
    float error_square = 0;

    float target_limit = 1200;
    float output_limit = 999;

    float output_inc = 0;
    float output = 0;
    float target = 0;

    float maximum = 0;
    float minimum = 0;

    cur_pid() {}

    cur_pid(float k1, float k2, float k3,
            float target_limit,
            float output_limit) : k1(k1),
                                  k2(k2),
                                  k3(k3),
                                  target_limit(target_limit),
                                  output_limit(output_limit) {}

    void set_pid(float k1_p, float k2_p, float k3_p)
    {
        k1 = k1_p;
        k2 = k2_p;
        k3 = k3_p;
    }

    void update(float current_point)
    {
        __Limit_Both(target, target_limit);
        // __Limit_AB(target, minimum, maximum);

        error_l = error;
        error = target - current_point;

        __Limit_Both(error, target_limit);
        // __Limit_AB(error, minimum, maximum);

        if (error > 0)
            error_square = 0.01 * error * error;
        else
            error_square = -0.01 * error * error;

        output_inc = k1 * error + k2 * (error - error_l) + k3 * error_square;

        // if (fabs(output_inc) < 5)
        //     output_inc = 0;

        output += output_inc;

        __Limit_Both(output, output_limit);
    }
};

#endif