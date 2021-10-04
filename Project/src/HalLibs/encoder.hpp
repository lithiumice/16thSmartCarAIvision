#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;
#include "hw_config.h"
#include "main.h"

class encoder
{
public:
    int index;
    float rpm = 0;
    float speed = 0;
    float encoder_pul_num = 0;
    float encoder_dis = 0;
    int inverse = 0;

    rt_tick_t last_time = 0;
    rt_tick_t current_time = 0;
    QTIMERN_enum qtimern;
    QTIMER_PIN_enum phaseA;
    QTIMER_PIN_enum phaseB;

    encoder() {}

    encoder(int index) : index(index)
    {
        if (1 == index)
        {
            qtimern = QTIMER_1;
            phaseA = QTIMER1_TIMER0_C0;
            phaseB = QTIMER1_TIMER1_C1;
            inverse = ENCODER_LEFT_INV;
        }
        else
        {
            qtimern = QTIMER_1;
            phaseA = QTIMER1_TIMER2_C2;
            phaseB = QTIMER1_TIMER3_C24;
            inverse = ENCODER_RIGHT_INV;
        }
    }

    void init()
    {
        qtimer_quad_init(qtimern, phaseA, phaseB);
    }

    float measure_speed()
    {
        current_time = rt_tick_get();
        encoder_pul_num = (float)qtimer_quad_get(qtimern, phaseA);
        qtimer_quad_clear(qtimern, phaseA);

        encoder_dis = encoder_pul_num / PULSE_PER_1CM;
        if (inverse)
            encoder_dis = -encoder_dis;

        speed = encoder_dis * 10000 / (current_time - last_time);

        last_time = current_time;
        return speed;
    }
};

#endif
