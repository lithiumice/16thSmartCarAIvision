#include "ctrl_chassis.h"
using namespace rtthread;
#include "main.h"

#include "ctrl_chassis.h"
#include "ctrl_inoutdev.h"
#include "ctrl_dymparam.h"
#include "ctrl_steer.h"

void chassis_ctrl::init()
{
    pos_pid left_pid = pos_pid(L_wheel_kp, L_wheel_ki, L_wheel_kd, 1200, -1200);
    pos_pid right_pid = pos_pid(R_wheel_kp, R_wheel_ki, R_wheel_kd, 1200, -1200);

    cur_pid L_cur = cur_pid(cur_k1, cur_k2, cur_k3, 1200, 999);
    cur_pid R_cur = cur_pid(cur_k1, cur_k2, cur_k3, 1200, 999);

    encoder left_encoder = encoder(1);
    encoder right_encoder = encoder(2);

    motor left_motor = motor(1);
    motor right_motor = motor(2);

    left_wheel = wheel(left_motor, left_encoder, left_pid, L_cur);
    right_wheel = wheel(right_motor, right_encoder, right_pid, R_cur);

    left_encoder.init();
    right_encoder.init();
    left_motor.init();
    right_motor.init();
    left_wheel.init();
    right_wheel.init();
}

void chassis_ctrl::stop()
{
    left_wheel.w_motor.run(0);
    right_wheel.w_motor.run(0);
}

void chassis_ctrl::update()
{
    left_wheel.update();
    right_wheel.update();
    for (int i = 0; i < 5; i++)
        speed_now[i] = (left_wheel.speed_now[i] + right_wheel.speed_now[i]) / 2;
}

float chassis_ctrl::row_to_distance(int target_row)
{
    int cur_row;
    int cur_dis;
    int pre_row;
    int pre_dis;
    float radio;
    for (int i = 1; i <= 17; i++)
    {
        cur_row = row_to_dis_table[2 * i];
        cur_dis = row_to_dis_table[2 * i + 1];

        pre_row = row_to_dis_table[2 * (i - 1)];
        pre_dis = row_to_dis_table[2 * (i - 1) + 1];

        if (target_row < cur_row)
        {
            radio = (float)(target_row - pre_row) / (cur_row - pre_row);
            return pre_dis + radio * (cur_dis - pre_dis);
        }
    }
    return 0;
}

void chassis_ctrl::set_speed(float set_speed, int steer_out)
{
    deg = -(float)steer_out * inoutdev.car_servo.max_angle / 1000; //inverse

    diff_value = set_speed * width * tan(deg * 3.14 / 180) / (2 * length);
    
    //__Limit_Both(diff_value,70);

    left_speed = set_speed + diff_value;
    right_speed = set_speed - diff_value;

    left_wheel.set_speed(left_speed);
    right_wheel.set_speed(right_speed);
}

void chassis_ctrl::update_speed()
{
    for (int i = 0; i <= 4; i++)
        speed_now[i] = (left_wheel.speed_now[i] + right_wheel.speed_now[i]) / 2;

    speed_ave = 0.8 * speed_now[0] + 0.1 * speed_now[1] + 0.1 * speed_now[2];
}

void chassis_ctrl::update_dis(int inverse)
{
    // float delta = speed_now[0] * 0.005;
    float delta = (left_wheel.w_encoder.encoder_dis + right_wheel.w_encoder.encoder_dis) / 2;
    if (inverse)
        delta = -delta;

    fork_cnt_distance += delta;
    allele_distance += delta;
    all_distance += delta;
    loop_distance += delta;
    fork_distance += delta;
    tag_distance += delta;
    start_distance += delta;
    ramp_distance += delta;
}

void chassis_ctrl::clear_dis()
{
    allele_distance = 0;
    all_distance = 0;
    loop_distance = 0;
    fork_distance = 0;
    tag_distance = 0;
    start_distance = 0;
    ramp_distance = 0;
}

void chassis_ctrl::start_brake()
{
    set_speed(0, 0);
    f1.chassis_update_type = 1;
    left_wheel.w_motor.run(-1000);
    right_wheel.w_motor.run(-1000);
}

void chassis_ctrl::hard_brake()
{
    left_wheel.get_speed();
    right_wheel.get_speed();
    update_speed();

    if (speed_now[0] > 2)
    {
        hard_bake_cnt = 0;
        left_wheel.w_motor.run(-1000);
        right_wheel.w_motor.run(-1000);
    }
    else
    {
        hard_bake_cnt++;
        if (hard_bake_cnt >= 3)
        {
            set_speed(0, 0);
            f1.chassis_update_type = 0;
        }
    }
}