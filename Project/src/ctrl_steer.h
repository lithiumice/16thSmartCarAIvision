#ifndef __STEER_H_
#define __STEER_H_
#include <rtthread.h>
using namespace rtthread;
#include "main.h"
#include "hw_config.h"
#include "common_macro.h"
#include "headfile.h"
#include "img_proc.h"
#include "f_elems.h"
#include "servo.hpp"
#include "pos_pid.hpp"

class steer_ctrl
{
public:
    float steer_fuzzy_kp_p = 7.2;
    float steer_basic_kp = 3.6;
    float steer_kd = 50;

    float steer_kp_j = 0.03;
    float steer_ki = 0;

    float steer_kp = 0;
    float steer_out = 0;
    int16 img_err_ave = 0;
    int16 img_err_history[30] = {0};
    pos_pid steer_err_pid;
    int img_err_diff = 0;

    steer_ctrl() {}

    void init();

    void solve_mid_error();

    void update_pid();

    void update_pid(int error);

    void update_fuzzy_pid();
};
extern steer_ctrl steer;

#endif