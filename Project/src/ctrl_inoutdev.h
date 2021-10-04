#ifndef __INOUT_H__
#define __INOUT_H__
#include <rtthread.h>
using namespace rtthread;
#include "main.h"
#include "hw_config.h"
#include "headfile.h"
#include "img_proc.h"
#include "f_elems.h"
#include "servo.hpp"
#include "kalman.hpp"
#include "quad_num.hpp"

#define IMUConverter 0.000532f

class inoutdev_ctrl
{
public:
    ebled_t errno_led, info_beep;
    agile_btn_t *key1, *key2, *key3, *key4, *key5, *key6, *key7;
    servo car_servo, art_servo;
    kalman_filter kalman_f;
    quad_num quad_n;
    _imu_st imu_data = {1, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0};

    uint32 current_time = 0;
    uint32 last_time = 0;
    uint32 fps = 0;

    float yaw_rate_200hz = 0;
    float gyro_z_filter_200hz = 0;
    float delta_z_200hz = 0;
    float gyro_y_filter_200hz = 0;
    float pitch_rate_200hz = 0;
    float delta_y_200hz = 0;

    float acc_x_filter = 0;
    float acc_y_filter = 0;
    float acc_z_filter = 0;

    float gyro_x_filter = 0;
    float gyro_z_filter = 0;
    float gyro_y_filter = 0;

    float gyro_z_offset = -6;
    float gyro_y_offset = -4;
    float gyro_x_offset = 0;

    float delta_z = 0;
    float delta_y = 0;

    float yaw_rate = 0;
    float true_yaw_rate = 0;
    float pitch_rate = 0;

    float yaw_angle = 0;
    float pitch_angle = 0;
    float roll_angle = 0;

    float gyro_y_omega = 0;
    float acc_angle = 0;
    float turn_angle = 0;

    float cross_z_angle = 0;
    float gyro_z_angle = 0;
    float gyro_y_angle = 0;
    float start_angle = 0;

    uint64 remote1_start_time;
    uint64 remote1_val;
    uint64 remote2_start_time;
    uint64 remote2_val;

    uint16 px[5][10] = {0}; //elec_raw_fifo

    inoutdev_ctrl() {}

    void init();
    void clock();
    void beep(int num);
    void lbeep(int num);
    void laser_on();
    void laser_off();
    void set_art_servo(int err);
    int switch_is_on(int num);

    void update_gyro_z();
    void update_gyro_y();
    void update_imu_kalman();

    int get_remote_ch1();
    int get_remote_ch2();
    int get_remote_channel();
    void update_imu_quad();
    void update_quad_z();

    void get_elec(int err);
};
extern inoutdev_ctrl inoutdev;
extern void btn_event_cb(agile_btn_t *btn);

#endif
