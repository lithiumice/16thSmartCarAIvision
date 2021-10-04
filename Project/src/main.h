#ifndef __MAIN_H_
#define __MAIN_H_

#include "common.h"
#include <rtthread.h>
using namespace rtthread;

#include "ctrl_chassis.h"
#include "ctrl_inoutdev.h"
#include "ctrl_dymparam.h"
#include "ctrl_steer.h"

#include "ano_dt.hpp"
#include "kalman.hpp"
#include "string"
#include "fuzzy.hpp"
#include "f_elems.h"
#include "img_proc.h"
#include "agile_led.h"
#include "string"
#include "servo.hpp"
#include "motor.hpp"
#include "wheel.hpp"
#include "encoder.hpp"
#include "pos_pid.hpp"
#include "inc_pid.hpp"
#include "cur_pid.hpp"
#include "display_ui.h"

////////////////////////////////
//任务代号
////////////////////////////////
enum anifriut_type
{
    animal,
    friut
};

enum chassis_status
{
    chassis_normal,
    chassis_hard_brake,
    chassis_stop,
    chassis_pos_loop,
};

enum balance_status
{
    balance_close_loop,
    balance_lie_down,
    balance_stand,
};

enum sch_type
{
    normal,
    regco,
    sch_pos_loop,
};

enum art_rev_type
{
    art_qrcode,
    art_fork_num,
    art_anifuit,
    art_rects
};

enum task_type
{
    servo_turn_around,
    laser_shoot,
    forward_30cm,
    backward_30cm,
    forward_10cm,
    back_ward_10cm
};

enum fork_sch_type
{
    fork_hard_brake,
    fork_backward_const_dis,
    fork_backward_dym_dis,
    fork_backward_slow,
    fork_pid_fix_pos,
    fork_wait_static,
    fork_analyze_num,
    fork_leaving_fork,
    fork_forward_slow
};

enum qrcode_sch_type
{
    qrcode_hard_brake,
    qrcode_back_off,
    qrcode_forward,
    qrcode_regco_qrcode,
    qrcode_move_slow,
    qrcode_wait_3s,
    qrcode_laser_shot,
    qrcode_leave_qrcode,
    qrcode_static,
    qrcode_forward_animal_dis,
    qrcode_forward_friut_dis,
};

////////////////////////////////
//常量定义
////////////////////////////////
#define ANI_FRIUT_SPEED 18
#define ANI_FRIUT_LASER_THRE 3
#define ANI_FRIUT_SERVO_TURN 850
#define LONG_STRAIGHT_ENDROW 45
#define LEFT_COMP 0
#define RIGHT_COMP 0

#ifdef USE_CAR_3
#define GYRO_INTERVAL 10      //1000 Hz
#define CURRENT_INTERVAL 5    //2 kHz
#define CHASSIS_INTERVAL 50   //200 Hz
#define ADC_BAT_INTERVAL 5000 //0.5 s
#else
#define GYRO_INTERVAL 10      //200 Hz
#define CURRENT_INTERVAL 5    //2 kHz
#define CHASSIS_INTERVAL 50   //200 Hz
#define ADC_BAT_INTERVAL 5000 //0.5 s
#endif

#ifdef USE_CAR_3
#define ENABLE_CROSS
#define ENABLE_CURVE
#define ENABLE_LOOP
#define ENABLE_FORK
// #define ENABLE_START
// #define ENABLE_RAMP
// #define ENABLE_GARAGE
// #define ENABLE_QRCODE
#else
#define ENABLE_CROSS
#define ENABLE_CURVE
#define ENABLE_LOOP
#define ENABLE_FORK
#define ENABLE_START
#define ENABLE_RAMP
// #define ENABLE_GARAGE
// #define ENABLE_QRCODE
#endif

////////////////////////////////
//函数声明
////////////////////////////////
void chassis_bake();
int str2bool(char *str);
void init_openart();
void laser_on();
void laser_off();
void start_regco(int type);
int clip(int x, int low, int up);
void btn_event_cb(agile_btn_t *btn);
void LPF_1_db(float hz, float time, float in, float *out);
float rt_millisecond_from_tick(rt_tick_t ticks);
float fclip(float x, float low, float up);
void push_and_pull(float *buff, int len, float newdata);
void push_int_pull(int *buff, int len, int newdata);

////////////////////////////////
//外部引用
////////////////////////////////
extern steer_fuzzy steer_f;
extern int16 MT9V03X_CFG_CSI[CONFIG_FINISH][2];
extern rt_timer_t current_timer;

#endif