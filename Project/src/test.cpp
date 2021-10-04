
#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;
#include "main.h"
#include "motor.hpp"
#include "wheel.hpp"
#include "encoder.hpp"
#include "pos_pid.hpp"
#include "inc_pid.hpp"
#include "servo.hpp"

motor *left_motor;
motor *right_motor;
encoder *left_encoder;
encoder *right_encoder;
servo *servo1;
servo *servo2;

void cinit()
{
    f1.chassis_update_type = 3;

    servo1 = new servo(1);
    servo2 = new servo(2);

    left_encoder = new encoder(1);
    right_encoder = new encoder(2);

    left_motor = new motor(1);
    right_motor = new motor(2);

    left_encoder->init();
    right_encoder->init();
    left_motor->init();
    right_motor->init();
    servo1->init();
    servo2->init();
}
MSH_CMD_EXPORT(cinit, cinit);

int cmotorl(int argc, char **argv)
{
    int motor_pwm = atoi(argv[1]);

    left_motor->run(motor_pwm);

    return RT_EOK;
}
MSH_CMD_EXPORT(cmotorl, cmotorl);

int cmotorr(int argc, char **argv)
{
    int motor_pwm = atoi(argv[1]);

    right_motor->run(motor_pwm);

    return RT_EOK;
}
MSH_CMD_EXPORT(cmotorr, cmotorr);

int cservo(int argc, char **argv)
{
    int servo_value = atoi(argv[1]);
    servo1->set(servo_value);

    return RT_EOK;
}
MSH_CMD_EXPORT(cservo, servo(-1000 ~1000));

int cservo2(int argc, char **argv)
{
    int servo_value = atoi(argv[1]);
    servo2->set(servo_value);

    return RT_EOK;
}
MSH_CMD_EXPORT(cservo2, cservo2(-1000 ~1000));

int cenc()
{
    while (1)
    {
        int a = (int)left_encoder->measure_speed();
        int b = (int)right_encoder->measure_speed();
        rt_kprintf("left: %d,right: %d\r\n", a, b);
        rt_thread_mdelay(50);
    }
}
MSH_CMD_EXPORT(cenc, enc);

int tenc()
{
    qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
    qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);

    while (1)
    {
        int a = (int)qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0);
        int b = (int)qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
        rt_kprintf("left: %d,right: %d\r\n", a, b);
        rt_thread_mdelay(50);
    }
}
MSH_CMD_EXPORT(tenc, tenc);

void tservo(int argc, char **argv)
{
    int set_duty = atoi(argv[1]);
    pwm_duty(SERVO1_PIN, set_duty * 5);
}
MSH_CMD_EXPORT(tservo, tservo);

void tservo2(int argc, char **argv)
{
    int set_duty = atoi(argv[1]);
    pwm_duty(SERVO2_PIN, set_duty * 5);
}
MSH_CMD_EXPORT(tservo2, tservo2);

void set_servo(int argc, char **argv)
{
    inoutdev.car_servo.mid_pos = atoi(argv[1]);
    inoutdev.car_servo.L_pos = atoi(argv[2]);
    inoutdev.car_servo.R_pos = atoi(argv[3]);
    inoutdev.car_servo.max_angle = atoi(argv[4]);
}
MSH_CMD_EXPORT(set_servo, set_servo);
