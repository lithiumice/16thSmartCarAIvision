#include "ctrl_dymparam.h"
#include <rtthread.h>
using namespace rtthread;

#include "ctrl_chassis.h"
#include "ctrl_inoutdev.h"
#include "ctrl_dymparam.h"
#include "ctrl_steer.h"

void dymparam_ctrl::dym_forsight()
{
    speed_real = 0.6 * chassis.speed_now[0] +
                 0.2 * chassis.speed_now[1] +
                 0.2 * chassis.speed_now[2];
    foresight = (int16)(fore_min + (float)(fore_max - fore_min) * (340 - speed_real) * (340 - speed_real) / (200 * 200));

    if (speed_real <= 100)
        foresight = fore_max;

    if (speed_real >= 300)
        foresight = fore_min;

    if (!f1.en_dym_foresight)
    {
        foresight = fore_fix;
    }
    if (f2.ramp_sm_state == 1 ||
        f2.ramp_sm_state == 2)
    {
        foresight = fore_max;
    }
    if(f2.fork_sm_state!=0&&f2.fork1_dir==0)
    {
        foresight=foresight+3;
    }

    __Limit_AB(foresight, fore_min, fore_max);

    if (f2.tag_sm_state != 0)
    {
        foresight = foresight - 8;
    }
}
void dymparam_ctrl::dym_type_speed()
{
    for (int i = 199; i >= 1; i--)
    {
        road_type[i] = road_type[i - 1];
    }

    speed_set = s_max_speed -
                abs(steer.img_err_history[0] - steer.img_err_history[5]) * dym_p2 -
                (s_max_speed - s_min_speed) * abs(steer.img_err_ave) / (dym_p1);

    __Limit_AB(speed_set, s_min_speed, s_max_speed);

    if ((f2.stop_car_flag && f1.en_stop_car&&f2.start_sm_state == 0) ||
        chassis_stop == f1.chassis_update_type)
    {
        road_type[0] = STOPED;
        speed_set = 0;
    }
    //坡道
    else if (f2.ramp_sm_state != 0)
    {
        road_type[0] = RAMP;
        speed_set = 240;
    }
    //二维码
    else if (f2.tag_sm_state != 0)
    {
        road_type[0] = TAG;
    }
    //出库
    else if (f2.start_sm_state != 0)
    {
        road_type[0] = START;
        // speed_set = 200;
        speed_set = 100;
    }else if(chassis.allele_distance<=50)
    {
        road_type[0] = START;
        speed_set=140;
    }
    //入库
    else if (f2.garage_sm_state != 0)
    {
        road_type[0] = GARBAGE;
        if (f1.current_turns == 2)
            speed_set = 130;
    }
    else if (f2.fork_sm_state != 0)
    {
        road_type[0] = FORK;
    }
    else if (f2.L_loop_sm_state != 0 ||
             f2.R_loop_sm_state != 0)
    {
        road_type[0] = LOOP;
        // speed_set = s_max_speed - 30;

        // if (
        //     f2.L_loop_sm_state == 4 ||
        //     f2.L_loop_sm_state == 5 ||
        //     f2.L_loop_sm_state == 6 ||
        //     f2.L_loop_sm_state == 7 ||
        //     f2.R_loop_sm_state == 4 ||
        //     f2.R_loop_sm_state == 5 ||
        //     f2.R_loop_sm_state == 7 ||
        //     f2.R_loop_sm_state == 6)
        // {
        //     speed_set = s_max_speed - 20;
        // }
    }
    else if (f2.long_straight_flag)
    {
        road_type[0] = LONG_STRAIGHT;
        speed_set = s_max_speed;
    }
    else if (f2.cross_flag == 1)
    {
        road_type[0] = CROSS;
    }
    else if (f2.shortstraight_flag)
    {
        road_type[0] = SHORT_STRAIGHT;
    }
    else if (f2.leftturnflag == 3 || f2.rightturnflag == 3)
    {
        road_type[0] = LARGE_CURVE;
        __Limit_AB(speed_set, s_min_speed, s_min_speed + 20);
    }
    else if (f2.leftturnflag == 1 || f2.rightturnflag == 1)
    {
        road_type[0] = SMALL_CURVE;
        __Limit_AB(speed_set, s_min_speed, s_min_speed + 30);
    }
    else
    {
        road_type[0] = NONE;
        __Limit_AB(speed_set, s_min_speed, s_min_speed + 20);
    }

    
    // if((f2.fork_num/2)%2==0&&f2.loop_num==(f2.fork_num/2)+1&&chassis.all_distance>870&&chassis.all_distance<1000)
    // {
    //     speed_set = 230;
    // }
    // if((f2.fork_num/2)%2==1&&f2.loop_num==(f2.fork_num/2)+1&&chassis.all_distance>220&&chassis.all_distance<380)
    // {
    //     speed_set = 230;
    // }
}

void dymparam_ctrl::dym_curve_speed(float h, float l, int p)
{
    speed_set = h - (h - l) * (steer.img_err_ave * steer.img_err_ave) / (p * p);
    __Limit_AB(speed_set, l, h);
}
