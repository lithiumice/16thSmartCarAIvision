#ifndef __SERVO_H__
#define __SERVO_H__
#include "common.h"
#include "zf_pwm.h"
#include "common_macro.h"
#include "hw_config.h"

class servo
{
public:
    int index = 1;

    PWMCH_enum pin;
    uint32 mid_pos = 0;
    uint32 L_pos = 0;
    uint32 R_pos = 0;
    uint32 half_one = 0;
    uint8 inverse = 0;
    int max_angle = 30;

    int set_duty = 0;

    servo() {}

    servo(int index) : index(index)
    {
        if (1 == index)
        {
            pin = SERVO1_PIN;
            mid_pos = SERVO1_MID_POS;
            L_pos = SERVO1_L_POS;
            R_pos = SERVO1_R_POS;
            half_one = SERVO1_HALF_ONE;
            inverse = SERVO1_INVERSE;
            max_angle = SERVO1_MAX_ANGLE;
        }
        else if (2 == index)
        {
            pin = SERVO2_PIN;
            mid_pos = SERVO2_MID_POS;
            L_pos = SERVO2_L_POS;
            R_pos = SERVO3_R_POS;
            half_one = SERVO2_HALF_ONE;
            inverse = SERVO2_INVERSE;
        }
    }

    void init()
    {
        if (1 == index)
        {
            pwm_init(pin, 50, mid_pos * 5);
        }
        else if (2 == index)
        {
            pwm_init(pin, 50, mid_pos * 5);
        }
    }

    void set(int angle)
    {
        if (inverse)
            angle = -angle;
        __Limit_Both(angle, 1000);

        //     int set_duty = mid_pos + (int)((float)angle / 1000 * half_one);
        if (angle > 0) //right
        {
            set_duty = mid_pos + (int)((float)angle / 1000 * (R_pos - mid_pos));
        }
        else
        {
            set_duty = mid_pos + (int)((float)angle / 1000 * (mid_pos - L_pos));
        }
        pwm_duty(pin, set_duty * 5);
    }
};

#endif
