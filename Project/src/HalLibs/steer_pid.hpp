#ifndef __POS_PID__
#define __POS_PID__

#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;

class steer_pid
{
public:
    float kp = 0;
    float ki = 0;
    float kd = 0;

    float error = 0;
    float error_last = 0;
    float error_diff = 0;

    float error_history[5];
    float error_diff_history[5];

    float p_error = 0;
    float i_error = 0;
    float d_error = 0;

    float last_out = 0;
    float output = 0;
    float target = 0;
    float intergral = 0;

    float anti_wind_radio = 0.5f;
    float anti_windup_value = 500;

    float maximum = 0;
    float minimum = 0;

    steer_pid() {}

    steer_pid(float kp, float ki, float kd, float maximum, float minimum) : kp(kp), ki(ki), kd(kd), maximum(maximum), minimum(minimum) {}

    void set_pid(float kp_p, float ki_p, float kd_p)
    {
        kp = kp_p;
        ki = ki_p;
        kd = kd_p;
    }

    void update(float current_point)
    {

        for (int i = 5; i >= 1; i--)
            error_history[i] = error_history[i - 1];
        error = target - current_point;
        error_history[0] = error;

        intergral += ki * error;
        __Limit_Both(intergral, anti_windup_value);

        for (int i = 5; i >= 1; i--)
            error_diff_history[i] = error_diff_history[i - 1];
        error_diff = (error - error_last);
        error_diff_history[0] = error_diff;

        p_error = kp * error;
        i_error = intergral;
        d_error = kd * error_diff;

        last_out = p_error + i_error + d_error;
        __Limit_AB(last_out, minimum, maximum);

        error_last = error;
        output = last_out;
    }
};

#endif