#ifndef __INC_PID__
#define __INC_PID__

#include "common_macro.h"
#include "rtthread.h"
using namespace rtthread;

class inc_pid
{
public:
    float kp = 0;
    float ki = 0;
    float kd = 0;
    float error = 0;
    float error_l = 0;
    float error_ll = 0;
    float p_error = 0;
    float i_error = 0;
    float d_error = 0;

    float last_out = 0;
    float output = 0;
    float target = 0;
    float maximum = 0;
    float minimum = 0;

    inc_pid() {}

    inc_pid(float kp, float ki, float kd, float maximum, float minimum)
        : kp(kp), ki(ki), kd(kd), maximum(maximum), minimum(minimum) {}

    void set_pid(float kp_p, float ki_p, float kd_p)
    {
        kp = kp_p;
        ki = ki_p;
        kd = kd_p;
    }

    void update(float current_point)
    {
        error = target - current_point;

        p_error = kp * (error - error_l);
        i_error = ki * error;
        d_error = kd * (error - 2 * error_l + error_ll);

        last_out += p_error + i_error + d_error;
        __Limit_AB(last_out, minimum, maximum);

        error_ll = error_l;
        error_l = error;
        output = last_out;
    }
};

#endif