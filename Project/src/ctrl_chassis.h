#ifndef __CHASSIS_H__
#define __CHASSIS_H__
#include <rtthread.h>
using namespace rtthread;
#include "main.h"
#include "hw_config.h"
#include "common_macro.h"
#include "headfile.h"
#include "img_proc.h"
#include "f_elems.h"
#include "wheel.hpp"

class chassis_ctrl
{
public:
    float width = 15.5;
    float length = 20.5;
    int hard_bake_cnt = 0;
    int16 pulse_left;
    int16 pulse_right;
    wheel left_wheel;
    wheel right_wheel;
    float deg = 0;
    float diff_value = 0;
    float left_speed = 0;
    float right_speed = 0;

    float speed_now[5] = {0};
    float speed_ave = 0;

    int L_crazy_flag = 0;
    int R_crazy_flag = 0;
    int crazy_cnt = 0;

    //关键变量

    float allele_distance = 0;
    float all_distance = 0;
    float loop_distance = 0;
    float fork_distance = 0;
    float fork_cnt_distance = 0;
    float tag_distance = 0;
    float start_distance = 0;
    float ramp_distance = 0;

    float cur_k1 = 0.75;
    float cur_k2 = 0.01;
    float cur_k3 = 0.0;

    float L_wheel_kp = 4.2;
    float L_wheel_ki = 0.3;
    float L_wheel_kd = 4.0;
    float R_wheel_kp = 4.2;
    float R_wheel_ki = 0.3;
    float R_wheel_kd = 4.0;
    // float L_wheel_kp = 4.2;
    // float L_wheel_ki = 0.15;
    // float L_wheel_kd = 4.0;
    // float R_wheel_kp = 4.2;
    // float R_wheel_ki = 0.15;
    // float R_wheel_kd = 4.0;


    int row_to_dis_table[34] = {
        //row,dis
        0, 200,
        30, 146, //start
        36, 111,
        42, 85,
        48, 71,
        54, 59,
        60, 51, //300cm/s
        66, 44,
        72, 37,
        78, 33, //74.2
        84, 30,
        90, 27,
        96, 24,
        102, 22,
        108, 19,
        114, 18, //end
        120, 13  //17
    };
    float spe_fore_k = 7.576;

    chassis_ctrl(){};

    void init();

    void stop();

    void update();

    float row_to_distance(int target_row);

    void set_speed(float set_speed, int steer_out);

    void clear_dis();

    void update_dis(int inverse = 0);
    void update_speed();
    void hard_brake();
    void start_brake();
};

extern chassis_ctrl chassis;

#endif
