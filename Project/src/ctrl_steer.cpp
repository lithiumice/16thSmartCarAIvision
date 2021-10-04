#include "ctrl_steer.h"
#include <rtthread.h>
using namespace rtthread;

#include "ctrl_chassis.h"
#include "ctrl_inoutdev.h"
#include "ctrl_dymparam.h"
#include "ctrl_steer.h"

void steer_ctrl::init()
{
    steer_err_pid = pos_pid(steer_kp, steer_ki, steer_kd, 1000, -1000);
    steer_err_pid.target = 0;
}

void steer_ctrl::solve_mid_error()
{
    float CenterSum = 0;
    float WeightSum = 0;
    for (int i = dymparam.foresight - 2;
         i < dymparam.foresight + 2; i++)
    {
        CenterSum += f1.midline[i];
        WeightSum++;
    }

    img_err_history[0] = (int)((CenterSum / WeightSum) - 80);
    __Limit_Both(img_err_history[0], 80);

    img_err_diff = img_err_history[0] - img_err_history[1];

    for (int i = 29; i >= 1; i--)
        img_err_history[i] = img_err_history[i - 1];

    img_err_ave = img_err_history[0];
}

void steer_ctrl::update_pid()
{
    steer_kp = steer_basic_kp + (img_err_ave * img_err_ave) * steer_kp_j;
    steer_err_pid.set_pid(steer_kp, steer_ki, steer_kd);
    steer_err_pid.update(img_err_ave);
    steer_out = steer_err_pid.output;
}

void steer_ctrl::update_pid(int error)
{
    steer_kp = steer_basic_kp + (error * error) * steer_kp_j;
    steer_err_pid.set_pid(steer_kp, steer_ki, steer_kd);
    steer_err_pid.update(error);
    steer_out = steer_err_pid.output;
}

void steer_ctrl::update_fuzzy_pid()
{
    steer_kp = steer_basic_kp + steer_f.fuzzy(img_err_ave, img_err_history[0] - img_err_history[3]) / steer_fuzzy_kp_p;

    if(f2.cross_flag)
      steer_kp+= 3.5;
    else if(f2.s_cross_flag)
      steer_kp+= 5.5;

    else if(f2.R_loop_flag)
    {
      steer_kp += 0.35;
      steer_kd -= 0.25;
    }

    // if(f2.R_loop_sm_state==5||
    // f2.R_loop_sm_state==4)
    // {
    //   steer_kp += 4.0;
    //   steer_kd += 1.0;
    // }

    steer_err_pid.set_pid(steer_kp, steer_ki, steer_kd);
    steer_err_pid.update(img_err_ave);

    if (f2.R_loop_sm_state==5||
        f2.R_loop_sm_state==4)
    {
        if (steer_err_pid.output>= -800)
            steer_err_pid.output = -800;
    }
    
    steer_out = steer_err_pid.output;
}