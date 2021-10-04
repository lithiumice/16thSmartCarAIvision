#ifndef __MWHEEL_H_
#define __MWHEEL_H_

#include "headfile.h"
#include "rtthread.h"
using namespace rtthread;
#include "motor.hpp"
#include "hw_config.h"
#include "encoder.hpp"
#include "pos_pid.hpp"
#include "inc_pid.hpp"
#include "cur_pid.hpp"

void LPF_1_db(float hz, float time, float in, float *out);

class wheel
{
public:
    motor w_motor;
    encoder w_encoder;
    pos_pid w_controller;
    cur_pid current_pid;

    uint16 current_offset = 0;
    float l_current_adc = 0;
    float current_adc = 0;
    float adc_history[5];

    float current_filted = 0;
    float adc_limit = 1200;

    float speed_now[5];
    float speed_ave;
    ADCCH_enum adc_pin;

    wheel() {}

    wheel(motor w_motor, encoder w_encoder, pos_pid w_controller, cur_pid current_pid)
        : w_motor(w_motor), w_encoder(w_encoder), w_controller(w_controller), current_pid(current_pid) {}

    void init()
    {
        if (w_motor.index == 1)
            adc_pin = ADC_L_PIN;
        else
            adc_pin = ADC_R_PIN;

        uint32 adc_sum = 0;
        uint32 adc_times = 0;
        for (int i = 0; i < 30; i++)
        {
            adc_times++;
            adc_sum += adc_mean_filter(ADC_1, adc_pin, 5);
            rt_thread_mdelay(2);
        }
        current_offset = (uint16)(adc_sum / adc_times);

        if (abs(current_offset - (2.5 * 0xFFF / 3.3)) > (1.0 * 0xFFF / 3.3))
            current_offset = (uint16)(2.5 * 0xFFF / 3.3);
    }

    void set_speed(float speed)
    {
        w_controller.target = speed;
    }

    void get_speed()
    {
        for (int i = 4; i >= 1; i--)
            speed_now[i] = speed_now[i - 1];
        speed_now[0] = w_encoder.measure_speed();
        speed_ave = 0.8 * speed_now[0] + 0.1 * speed_now[1] + 0.1 * speed_now[2];
    }

    void update()
    {
        get_speed();

        // #ifdef USE_CAR_3
        //         speed_ave = speed_ave / 2;
        // #endif
        w_controller.update(speed_ave);
        current_pid.target = w_controller.output;
    }

    void update_current()
    {

        for (int i = 4; i >= 1; i--)
            adc_history[i] = adc_history[i - 1];

        uint16 value = adc_mean_filter(ADC_1, adc_pin, 2);
        current_adc = (float)(value - current_offset) * 3300 / 0xFFF;
        __Limit_Both(current_adc, adc_limit);
        adc_history[0] = current_adc;

#if 0
        current_adc = 0.8 * current_adc + 0.2 * l_current_adc;
        current_pid.update(current_adc);
#else
        current_adc = 0.6 * adc_history[0] +
                      0.1 * adc_history[1] +
                      0.1 * adc_history[2] +
                      0.1 * adc_history[3] +
                      0.1 * adc_history[4];
        LPF_1_db(35, 3000, current_adc, &current_filted);
        current_pid.update(current_filted);
#endif
        w_motor.run((int)current_pid.output);
        l_current_adc = current_adc;
    }
};
#endif