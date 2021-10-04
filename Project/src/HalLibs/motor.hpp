#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;

//left: 50; right:
#define MOTOR_PWM_FREQ 17000 //12500 //20000
class motor
{
public:
    int index;

    motor() {}

    motor(int index) : index(index) {}

    void init()
    {
        if (1 == index)
        {
            pwm_init(PWM1_MODULE3_CHA_D0, MOTOR_PWM_FREQ, 0);
            pwm_init(PWM1_MODULE3_CHB_D1, MOTOR_PWM_FREQ, 0);
        }
        else
        {
            pwm_init(PWM2_MODULE3_CHA_D2, MOTOR_PWM_FREQ, 0);
            pwm_init(PWM2_MODULE3_CHB_D3, MOTOR_PWM_FREQ, 0);
        }
    }

    void run(int thousands)
    {
        __Limit_Both(thousands, 1000);
        thousands *= 50;

        if (1 == index)
        {
            if (thousands >= 0)
            {
                pwm_duty(PWM1_MODULE3_CHA_D0, 0);
                pwm_duty(PWM1_MODULE3_CHB_D1, thousands);
            }
            else
            {
                thousands = -thousands;
                pwm_duty(PWM1_MODULE3_CHA_D0, thousands);
                pwm_duty(PWM1_MODULE3_CHB_D1, 0);
            }
        }
        else
        {
            if (thousands >= 0)
            {
                pwm_duty(PWM2_MODULE3_CHA_D2, thousands);
                pwm_duty(PWM2_MODULE3_CHB_D3, 0);
            }
            else
            {
                thousands = -thousands;
                pwm_duty(PWM2_MODULE3_CHA_D2, 0);
                pwm_duty(PWM2_MODULE3_CHB_D3, thousands);
            }
        }
    }
};

#endif
