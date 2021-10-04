#include "hw_config.h"
#ifdef USE_CAR_ZL

#include "main.h"
#include "PS2.h"

rt_sem_t csi_done_sem = RT_NULL;
rt_timer_t disp_timer;
rt_timer_t current_timer;

chassis_ctrl chassis;
inoutdev_ctrl inoutdev;
dymparam_ctrl dymparam;
imgproc_ctrl img_proc_c;
steer_ctrl steer;
data_send ano;
rt_sem_t ano_upload_sem = RT_NULL;

// pos_pid dir_outter;
pos_pid dir_inner;
pos_pid angle_loop;
pos_pid speed_loop;

// float mechanical_mid_angle = 25.0;
// float mechanical_liedown_angle = 5.4;
// float mechanical_backdown_angle = 54.3;

float mechanical_mid_angle = 33.0;
float mechanical_liedown_angle = 7.15;
float mechanical_backdown_angle = 56.3;

float img_err_k1 = 2.8;
float speed_ave = 0;

float img_err = 0;
float speed_target = 0;
float cur_speed_output = 0;
float last_speed_output = 0;

void ano_thread(void *parameter)
{
    while (1)
    {
        rt_sem_take(ano_upload_sem, RT_WAITING_FOREVER);
        rt_sem_control(ano_upload_sem, RT_IPC_CMD_RESET, RT_NULL);

        if (inoutdev.switch_is_on(5))
            ano.send_vofa_8(
                (float)speed_ave,
                (float)speed_loop.target,
                (float)inoutdev.pitch_angle,
                (float)angle_loop.target,
                (float)dir_inner.target,
                (float)inoutdev.true_yaw_rate,
                0,
                0);
    }
}

void adc_bat_thread(void *parameters)
{
    uint16 value = adc_mean_filter(ADC_1, ADC1_CH3_B14, 5);
    f1.bat_voltage = (float)value * 3.3 / 0xFFF * 5.7;

    static int cnt = 0;
    cnt++;
    if (cnt % 3 == 0)
        chassis.crazy_cnt = 0;
}

void current_thread(void *parameters)
{
    if (chassis_normal == f1.chassis_update_type)
    {
        chassis.left_wheel.update_current();
        chassis.right_wheel.update_current();
    }
    else if (chassis_stop == f1.chassis_update_type)
    {
        chassis.stop();
    }
}

void speed_loop_thread(void *parameter)
{
    chassis.left_wheel.get_speed();
    chassis.right_wheel.get_speed();
    chassis.update_speed();
    chassis.update_dis(1);

    if (chassis_normal == f1.chassis_update_type)
    {
        if (
            // fabs(chassis.left_wheel.speed_now[0] - chassis.left_wheel.speed_now[1]) > 5 ||
            fabs(chassis.left_wheel.speed_now[0]) > 400)
            chassis.L_crazy_flag = 1;
        else
            chassis.L_crazy_flag = 0;

        if (
            // fabs(chassis.right_wheel.speed_now[0] - chassis.right_wheel.speed_now[1]) > 5 ||
            fabs(chassis.right_wheel.speed_now[0]) > 400)
            chassis.R_crazy_flag = 1;
        else
            chassis.R_crazy_flag = 0;

        if (chassis.L_crazy_flag || chassis.R_crazy_flag)
            chassis.crazy_cnt++;

        if (chassis.crazy_cnt >= 200)
        {
            f1.chassis_update_type = chassis_stop;
            chassis.crazy_cnt = 0;
            chassis.L_crazy_flag = 0;
            chassis.R_crazy_flag = 0;
        }

        static int cnt = 0;
        static int cnt2 = 0;

        cnt++;
        if (cnt >= 2)
        {
            cnt = 0;

            cnt2++;
            if (cnt2 >= 10)
            {
                cnt2 = 0;
                last_speed_output = speed_loop.output;
                speed_ave = 0.8 * chassis.speed_now[0] + 0.1 * chassis.speed_now[1] + 0.1 * chassis.speed_now[2];
                speed_loop.target = speed_target;
                speed_loop.update(speed_ave);
            }
            cur_speed_output = last_speed_output + (speed_loop.output - last_speed_output) * cnt2 / 10;
        }
    }
    else if (chassis_stop == f1.chassis_update_type)
    {
    }
    rt_sem_release(ano_upload_sem);
}

void angle_loop_thread(void *parameter)
{
    get_icm20602_gyro_spi();
    get_icm20602_accdata_spi();
    inoutdev.update_imu_quad();
    inoutdev.update_quad_z();

    if (balance_close_loop == f1.balance_update_type)
    {
        angle_loop.target = mechanical_mid_angle - cur_speed_output;
    }
    else if (balance_stand == f1.balance_update_type)
    {
        angle_loop.target = mechanical_mid_angle;
    }
    else if (balance_lie_down == f1.balance_update_type)
    {
        angle_loop.target = mechanical_liedown_angle;
    }

    if (chassis_normal == f1.chassis_update_type)
    {
        angle_loop.update(inoutdev.pitch_angle, 2);
#if 1
        // if (dir_inner.output > 0)
        // {
        //     chassis.left_wheel.current_pid.target = angle_loop.output + dir_inner.output;
        //     chassis.right_wheel.current_pid.target = angle_loop.output;
        // }
        // else
        // {
        //     chassis.left_wheel.current_pid.target = angle_loop.output;
        //     chassis.right_wheel.current_pid.target = angle_loop.output + dir_inner.output;
        // }
        chassis.left_wheel.current_pid.target = (angle_loop.output + dir_inner.output);
        chassis.right_wheel.current_pid.target = (angle_loop.output - dir_inner.output);
#else
        chassis.left_wheel.current_pid.target = angle_loop.output;
        chassis.right_wheel.current_pid.target = angle_loop.output;
#endif
        float dir_speed = speed_ave;
        __Limit_AB(dir_speed, 100, 400);
        dir_inner.target = dir_speed * 0.01 * img_err * img_err_k1; //1~3.5*img_err

        dir_inner.update(inoutdev.true_yaw_rate, 2);
    }
}

// void dir_inner_thread(void *parameter)
// {
//     if (chassis_normal == f1.chassis_update_type)
//     {
//         // float dir_speed;
//         // dir_speed = speed_ave;
//         // __Limit_AB(dir_speed, 100, 400);
//         // dir_inner.target = dir_speed * 0.01 * img_err * img_err_k1; //1~3.5*img_err

//         dir_inner.update(inoutdev.true_yaw_rate, 2);
//     }
// }

int main(void)
{
#if 1
    MT9V03X_CFG_CSI[0][1] = f1.cam_auto_exp;   //自动曝光
    MT9V03X_CFG_CSI[1][1] = f1.cam_bright_num; //曝光度
    MT9V03X_CFG_CSI[2][1] = f1.cam_fps;        //帧率

    dir_inner = pos_pid(10, 0.1, 0, 550, -550);
    angle_loop = pos_pid(700, 0, 2000, 800, -800);
    speed_loop = pos_pid(0.02, 0.01, 0,
                         mechanical_mid_angle - mechanical_liedown_angle - 8,
                         mechanical_mid_angle - mechanical_backdown_angle + 6);
    // speed_loop = pos_pid(0.018, 0.0002, 0, 16, -13);
    csi_done_sem = rt_sem_create("csi_done_sem", 0, RT_IPC_FLAG_FIFO);
    ano_upload_sem = rt_sem_create("ano_upload_sem", 0, RT_IPC_FLAG_FIFO);
    dymparam.foresight = 81;//220
    // dymparam.foresight = 80;//200
    dymparam.foresight = 77; //250

    inoutdev.init();
    chassis.init();
    ano.init(115200);

    rt_thread_t tid_ano = rt_thread_create("ano_thread", ano_thread, RT_NULL, 2048, 30, 3);
    rt_thread_startup(tid_ano);

    rt_timer_t bat_timer = rt_timer_create("bat_timer", adc_bat_thread, RT_NULL, ADC_BAT_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(bat_timer);

    rt_timer_t gyro_timer = rt_timer_create("angle_loop_thread", angle_loop_thread, RT_NULL, GYRO_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(gyro_timer);

    // rt_timer_t dir_inner_timer = rt_timer_create("dir_inner_thread", dir_inner_thread, RT_NULL, GYRO_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    // rt_timer_start(dir_inner_timer);

    current_timer = rt_timer_create("current_timer", current_thread, RT_NULL, CURRENT_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(current_timer);

    rt_timer_t speed_timer = rt_timer_create("speed_loop_thread", speed_loop_thread, RT_NULL, CHASSIS_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(speed_timer);

    rt_thread_t tid_display = rt_thread_create("display_thread", display_thread, RT_NULL, 2048, 31, 5);
    rt_thread_startup(tid_display);

#if 1
    while (1)
    {

        rt_sem_take(csi_done_sem, RT_WAITING_FOREVER);
        rt_sem_control(csi_done_sem, RT_IPC_CMD_RESET, RT_NULL);
        inoutdev.clock();

        f1.start_time1 = pit_get_ms(PIT_CH0);

        img_proc_c.img_resize((uint8 **)mt9v03x_csi_image, (uint8 **)gray_120_160_image, 240, 320, 2);
        img_proc_c.gray_to_binary((uint8 **)gray_120_160_image, (uint8 **)bin_120_160_image, 120, 160, 1);

        find_track_gray();
        find_elements();

        dymparam.dym_type_speed();
        steer.solve_mid_error();
        img_err = steer.img_err_ave;

        if (inoutdev.switch_is_on(8))
        {
                    speed_target = -220;
}else
{
                    speed_target = 0;
} 

        if (inoutdev.switch_is_on(2))
        {
            img_err = -(float)inoutdev.get_remote_ch1() / 1000 * 80;
            speed_target = -(float)inoutdev.get_remote_ch2() / 1000 * 200;
        }

        f1.end_time1 = pit_get_ms(PIT_CH0);
        f1.use_time1 = f1.end_time1 - f1.start_time1;
    }
#endif
#endif
}
EX_UPPER_F(set_art_pid_param_cmd,
           angle_loop.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));)

EX_UPPER_F(set_steer_fuzzy_p,
           img_err_k1 = atoi(argv[1]);)

EX_UPPER_F(set_remote_stop,
           img_err = atoi(argv[1]);)

EX_UPPER_F(set_cur_pid_param_cmd,
           dymparam.foresight = atoi(argv[1]);
           //    chassis.right_wheel.current_pid.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));
)

EX_UPPER_F(set_chassis_pid_param_cmd,
           speed_loop.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));
           dir_inner.set_pid(atof(argv[4]), atof(argv[5]), atof(argv[6]));)

#endif