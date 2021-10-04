#include "ctrl_inoutdev.h"
#include "ctrl_chassis.h"
#include "ctrl_dymparam.h"
#include "ctrl_steer.h"
#include "common_macro.h"
#include <rtthread.h>
using namespace rtthread;
#include "zf_gpio.h"

void GPIO2_Combined_0_15_IRQHandler(void)
{
    rt_interrupt_leave();
    if (GET_GPIO_FLAG(REMOTE_PIN1))
    {
        CLEAR_GPIO_FLAG(REMOTE_PIN1); //清除中断标志位
        if (gpio_get(REMOTE_PIN1) == GPIO_HIGH)
        {
            inoutdev.remote1_start_time = pit_get_us(PIT_CH1);
        }
        else
        {
            inoutdev.remote1_val = pit_get_us(PIT_CH1) - inoutdev.remote1_start_time;
        }
    }
    if (GET_GPIO_FLAG(REMOTE_PIN2))
    {
        CLEAR_GPIO_FLAG(REMOTE_PIN2); //清除中断标志位
        if (gpio_get(REMOTE_PIN2) == GPIO_HIGH)
        {
            inoutdev.remote2_start_time = pit_get_us(PIT_CH1);
        }
        else
        {
            inoutdev.remote2_val = pit_get_us(PIT_CH1) - inoutdev.remote2_start_time;
        }
    }
    rt_interrupt_enter();
}

int inoutdev_ctrl::get_remote_ch1()
{
    //-1000-1000,(remote1_val - 1500) * 1000 / (2000-1000);
    int ret = (remote1_val - 1500) * 2;
    __Limit_Both(ret, 1000);
    return ret;
}

int inoutdev_ctrl::get_remote_ch2()
{
    int ret = (remote2_val - 1500) * 2;
    __Limit_Both(ret, 1000);
    return ret;
}

int inoutdev_ctrl::get_remote_channel()
{
    int ret = (remote2_val - 1500) * 2;
    __Limit_Both(ret, 1000);
    return ret;
}

void inoutdev_ctrl::init()
{
    lcd_init();
    lcd_big_str(3, 2, RED, WHITE, "STOPED", 0);

    car_servo = servo(1);
    art_servo = servo(2);

    car_servo.init();
    art_servo.init();

    pit_init();
    pit_start(PIT_CH0);
    pit_start(PIT_CH1);
    pit_interrupt_us(PIT_CH2, 1000);

    mt9v03x_csi_init();
    icm20602_init_spi();

    adc_init(ADC_1, ADC_L_PIN, ADC_12BIT);
    adc_init(ADC_1, ADC_R_PIN, ADC_12BIT);

    adc_init(ADC_1, ADC_1_PIN, ADC_12BIT);
    adc_init(ADC_1, ADC_2_PIN, ADC_12BIT);
    adc_init(ADC_1, ADC_3_PIN, ADC_12BIT);

    adc_init(ADC_1, ADC1_CH3_B14, ADC_12BIT);
    pwm_init(LASER_PIN, 125, 0);

    errno_led = easyblink_init_led(LED0_PIN, PIN_LOW);
    info_beep = easyblink_init_led(BEEP_PIN, PIN_HIGH);
    easyblink(errno_led, -1, 200, 1000);

    gpio_init(SWITCH_1, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_2, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_3, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_4, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_5, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_6, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_7, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_8, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);

    key1 = agile_btn_create(KEY1_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key2 = agile_btn_create(KEY2_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key3 = agile_btn_create(KEY3_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key4 = agile_btn_create(KEY4_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key5 = agile_btn_create(KEY5_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key6 = agile_btn_create(KEY6_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    key7 = agile_btn_create(KEY7_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP);

    agile_btn_set_event_cb(key1, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key2, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key3, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key4, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key5, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key6, BTN_CLICK_EVENT, btn_event_cb);
    agile_btn_set_event_cb(key7, BTN_CLICK_EVENT, btn_event_cb);

    agile_btn_start(key1);
    agile_btn_start(key2);
    agile_btn_start(key3);
    agile_btn_start(key4);
    agile_btn_start(key5);
    agile_btn_start(key6);
    agile_btn_start(key7);

    gpio_init(REMOTE_PIN1, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(REMOTE_PIN2, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);

    gpio_interrupt_init(REMOTE_PIN1, BOTH, GPIO_INT_CONFIG);
    gpio_interrupt_init(REMOTE_PIN2, BOTH, GPIO_INT_CONFIG);
    // NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, 15); //设置中断优先级 范围0-15 越小优先级越高
}
void inoutdev_ctrl::clock()
{
    current_time = pit_get_ms(PIT_CH0);
    fps = 1000 / (current_time - last_time);
    last_time = current_time;
}
void inoutdev_ctrl::beep(int num)
{
    easyblink(info_beep, num, 30, 80);
}

void inoutdev_ctrl::lbeep(int num)
{
    easyblink(info_beep, num, 200, 400);
}
void inoutdev_ctrl::laser_on()
{
    pwm_duty(LASER_PIN, PWM_DUTY_MAX * 0.5);
}
void inoutdev_ctrl::laser_off()
{
    pwm_duty(LASER_PIN, 0);
}

void inoutdev_ctrl::set_art_servo(int err)
{
    art_servo.set(-(err)*500 / 70);
}

// void inoutdev_ctrl::get_elec(int err)
// {
//     uint16 sum[5] = {0};
//     uint16 ad_value[5] = {0};
//     // uint16 temp = 0;

//     for (int i = 0; i < 5; i++)
//     {
//         *(*(px + 0) + i) = adc_convert(ADC_1, ADC_1_PIN);
//         *(*(px + 1) + i) = adc_convert(ADC_1, ADC_2_PIN);
//         *(*(px + 2) + i) = adc_convert(ADC_1, ADC_3_PIN);
//     }

//     for (int i = 0; i < 3; i++)
//     {
//         for (int j = 0; j < 5; j++)
//         {
//             for (int k = 0; k < 5 - j; k++)
//             {
//                 if ((*(*(px + i) + k)) < (*(*(px + i) + k + 1)))
//                 {
//                     temp = *(*(px + i) + k + 1);
//                     *(*(px + i) + k + 1) = *(*(px + i) + k);
//                     *(*(px + i) + k) = temp;
//                 }
//             }
//         }
//     }

//     for (int i = 0; i < 5; i++)
//     {
//         *(sum + i) = (*(*(px + i) + 1)) + (*(*(px + i) + 2)) + (*(*(px + i) + 3));
//         *(ad_value + i) = (*(sum + i)) / 3;
//     }
//     /*===========================加权平均滤波=================================*/
//     for (i = 0; i < 3; i++)
//     {
//         //            *(*(ad_value_temp+i)+4)=*(*(ad_value_temp+i)+3);
//         //            *(*(ad_value_temp+i)+3)=*(*(ad_value_temp+i)+2);
//         //            *(*(ad_value_temp+i)+2)=*(*(ad_value_temp+i)+1);
//         //            *(*(ad_value_temp+i)+1)=*(*(ad_value_temp+i)+0);
//         //            *(*(ad_value_temp+i)+0)=*(ad_value+i);

//         SensorVal.AdValue.Ad_avr_val[i] = *(ad_value + i) / 10 * 10;
//         //date.original_ad[i]=(uint16)((*(*(ad_value_temp+i)+0))*0.5f+(*(*(ad_value_temp+i)+1))*0.3f+(*(*(ad_value_temp+i)+2))*0.2f)/10*10;
//         SensorVal.AdValue.Ad_avr_val[i] = (SensorVal.AdValue.Ad_avr_val[i] < 10 ? 10 : SensorVal.AdValue.Ad_avr_val[i]);
//     }

//     SensorVal.AdValue.Ad_avr_val[3] = (adc_once(ADC1_SE12, ADC_12bit)) * 10 / 10; //3
//     SensorVal.AdValue.Ad_avr_val[3] = (SensorVal.AdValue.Ad_avr_val[3] < 10 ? 10 : SensorVal.AdValue.Ad_avr_val[3]);
//     SensorVal.AdValue.Ad_avr_val[4] = (adc_once(ADC1_SE11, ADC_12bit)) * 10 / 10; //4
//     SensorVal.AdValue.Ad_avr_val[4] = (SensorVal.AdValue.Ad_avr_val[4] < 10 ? 10 : SensorVal.AdValue.Ad_avr_val[4]);
// }

void inoutdev_ctrl::update_gyro_z()
{
    LPF_1_db(35, 1000, (float)(icm_gyro_z - gyro_z_offset), &gyro_z_filter_200hz);

    yaw_rate_200hz = -0.03051757f * gyro_z_filter_200hz;
    if (yaw_rate_200hz < 1.3 && yaw_rate_200hz > -1.3)
        yaw_rate_200hz = 0;

    delta_z_200hz = 10.3 * yaw_rate_200hz / 1000.0;
    gyro_z_angle += delta_z_200hz;
    start_angle += delta_z_200hz;

    if (fabs(gyro_z_angle) >= 360)
        gyro_z_angle = 0;
}

void inoutdev_ctrl::update_gyro_y()
{
    LPF_1_db(35, 1000, (float)(icm_gyro_y - gyro_y_offset), &gyro_y_filter_200hz);

    pitch_rate_200hz = -0.03051757f * gyro_y_filter_200hz;
    if (pitch_rate_200hz < 0.3 && pitch_rate_200hz > -0.3)
        pitch_rate_200hz = 0;

    delta_y_200hz = 10.3 * pitch_rate_200hz / 1000.0;
    gyro_y_angle += delta_y_200hz;
    pitch_angle += delta_y_200hz;
    // LPF_1_db(35, 1000, (float)(icm_gyro_x), &gyro_y_filter_200hz);

    // pitch_rate_200hz = -0.03051757f * gyro_y_filter_200hz;
    // if (pitch_rate_200hz < 0.3 && pitch_rate_200hz > -0.3)
    //     pitch_rate_200hz = 0;

    // delta_y_200hz = 10.3 * pitch_rate_200hz / 1000.0;
    // gyro_y_angle += delta_y_200hz;
    // pitch_angle += delta_y_200hz;
}

void inoutdev_ctrl::update_imu_kalman()
{

    LPF_1_db(35, 1000, (float)(icm_gyro_z - gyro_z_offset), &gyro_z_filter);

    LPF_1_db(35, 1000, (float)(icm_gyro_y), &gyro_y_filter);
    // LPF_1_db(35, 1000, (float)(icm_gyro_y - gyro_y_offset), &gyro_y_filter);

    LPF_1_db(35, 1000, (float)(icm_acc_z), &acc_z_filter);
    // float faiZ = acc_z_filter / 16384;
    // __Limit_Both(faiZ, 1);
    // acc_angle = asin(faiZ) / 0.01745329;
    float faiZ = (acc_z_filter + 700) * 180 / 16384;
    __Limit_Both(faiZ, 40);
    acc_angle = -asin(acc_z_filter / -8192.0f) * 57.29578; //计算真实角度用作前瞻计算
    // ramp_omega = (int)(mycos(realangle*0.0174533) * Komega);

    // static uint8 flag_zero = 0;
    // if (flag_zero == 0) //防止收敛到错误值
    // {
    //     kalman_f.Angle = acc_angle;
    //     flag_zero = 1;
    // }

    gyro_y_omega = -0.03051757f * gyro_y_filter;
    yaw_rate = -0.03051757f * gyro_z_filter;

    kalman_f.filter(gyro_y_omega, acc_angle);

    pitch_angle = kalman_f.Angle;
    pitch_rate = kalman_f.Gyro_x;
    true_yaw_rate = -yaw_rate / cos(pitch_angle * (PI / 180));
}

void inoutdev_ctrl::update_imu_quad()
{
    LPF_1_db(35, 1000, (float)(icm_gyro_z - gyro_z_offset), &gyro_z_filter);
    LPF_1_db(35, 1000, (float)(icm_gyro_y - gyro_y_offset), &gyro_y_filter);
    LPF_1_db(35, 1000, (float)(icm_gyro_x - gyro_x_offset), &gyro_x_filter);

    LPF_1_db(35, 1000, (float)(icm_acc_x), &acc_x_filter);
    LPF_1_db(35, 1000, (float)(icm_acc_y), &acc_y_filter);
    LPF_1_db(35, 1000, (float)(icm_acc_z), &acc_z_filter);

// acc_x_filter=icm_acc_x;
// acc_y_filter=icm_acc_y;
// acc_z_filter=icm_acc_z;
// gyro_x_filter=icm_gyro_x - gyro_x_offset;
// gyro_y_filter=icm_gyro_y - gyro_y_offset;
// gyro_z_filter=icm_gyro_z - gyro_z_offset;
    quad_n.IMUupdate(IMUConverter * gyro_x_filter,
                     IMUConverter * gyro_y_filter,
                     IMUConverter * gyro_z_filter,
                     acc_x_filter, acc_y_filter, acc_z_filter, &imu_data);

    // quad_n.IMUupdate(IMUConverter * gyro_x_filter,
    //                  IMUConverter * gyro_y_filter,
    //                  IMUConverter * gyro_z_filter,
    //                  acc_x_filter, 0, acc_z_filter, &imu_data);

    pitch_angle = -imu_data.pit;
    yaw_angle = imu_data.yaw;
    roll_angle = imu_data.rol;

    pitch_rate = 0.03051757f * gyro_y_filter;
    yaw_rate = 0.03051757f * gyro_z_filter;
    delta_y = pitch_rate * 0.001;

    true_yaw_rate = -yaw_rate / cos(pitch_angle * (PI / 180));
    delta_z = 2 * true_yaw_rate * 0.001;
    turn_angle += delta_z;

    gyro_y_angle = pitch_angle;
    // gyro_y_angle += delta_y_200hz;
    //     pitch_angle += delta_y_200hz;
}

void inoutdev_ctrl::update_quad_z()
{
    gyro_z_angle += delta_z;
    start_angle += delta_z;
    if (fabs(gyro_z_angle) >= 360)
        gyro_z_angle = 0;
}

int inoutdev_ctrl::switch_is_on(int num)
{
    if (num == 1)
        return 0 == gpio_get(SWITCH_1);
    else if (num == 2)
        return 0 == gpio_get(SWITCH_2);
    else if (num == 3)
        return 0 == gpio_get(SWITCH_3);
    else if (num == 4)
        return 0 == gpio_get(SWITCH_4);
    else if (num == 5)
        return 0 == gpio_get(SWITCH_5);
    else if (num == 6)
        return 0 == gpio_get(SWITCH_6);
    else if (num == 7)
        return 0 == gpio_get(SWITCH_7);
    else if (num == 8)
        return 0 == gpio_get(SWITCH_8);
    return RT_ERROR;
}