#include "hw_config.h"
//#if defined (USE_CAR_1)

#include "main.h"
#include "PS2.h"

rt_sem_t csi_done_sem = RT_NULL;
rt_sem_t ano_upload_sem = RT_NULL;
rt_timer_t disp_timer;
rt_timer_t current_timer;
 
steer_ctrl steer;
chassis_ctrl chassis;
inoutdev_ctrl inoutdev;
dymparam_ctrl dymparam;
imgproc_ctrl img_proc_c;
steer_fuzzy steer_f;
rt_mutex_t chassis_mutex;
cur_pid art_pid;
pos_pid pos_loop;
pos_pid pos_loop_two;
data_send ano;

void adc_bat_thread(void *parameters)
{
    uint16 value = adc_mean_filter(ADC_1, ADC_BAT_PIN, 5);
    f1.bat_voltage = (float)value * 3.3 / 0xFFF * 5.7;

    // f2.garage_dir = inoutdev.switch_is_on(1);
    // f1.en_dis_ele = inoutdev.switch_is_on(2);

    // if (inoutdev.switch_is_on(7))
    // {
    //     dymparam.s_max_speed = 380;
    //     dymparam.s_min_speed = 280;
    //     dymparam.fore_min = 61;
    // }
    // else
    // {
    //     dymparam.s_max_speed = 370;
    //     dymparam.s_min_speed = 270;
    //     dymparam.fore_min = 62;
    // }

    if (inoutdev.switch_is_on(8))
    {
        f2.sch_sm_state = regco;
    }
    else
    {
        f2.sch_sm_state = normal;
    }
}

void current_thread(void *parameters)
{
    if (chassis_normal == f1.chassis_update_type)
    {
        chassis.left_wheel.update_current();
        chassis.right_wheel.update_current();
    }
    else if (3 == f1.chassis_update_type)
    {
        chassis.left_wheel.w_motor.run(0);
        chassis.right_wheel.w_motor.run(0);
    }
    else if (chassis_hard_brake == f1.chassis_update_type)
    {
    }
}   

void chassis_thread(void *parameter)
{
    if (chassis_normal == f1.chassis_update_type)
    {
        chassis.left_wheel.update();
        chassis.right_wheel.update();
        chassis.update_speed();
    }
    else if (3 == f1.chassis_update_type)
    {
        chassis.left_wheel.get_speed();
        chassis.right_wheel.get_speed();
        chassis.update_speed();

    }
    else if (chassis_hard_brake == f1.chassis_update_type)
    {
        chassis.hard_brake();
    }

    chassis.update_dis();
}

void gyro_thread(void *parameter)
{
    get_icm20602_gyro_spi();
    get_icm20602_accdata_spi();

    static int cnt = 0;
    cnt++;
    if (cnt >= 5)
    {
        cnt = 0;
        inoutdev.update_gyro_y();
        inoutdev.update_gyro_z();
    }

    //  inoutdev.update_imu_quad();
    // inoutdev.update_quad_z();
}

void normal_run()
{
    find_track_gray();
    find_elements();

    dymparam.dym_forsight();
    dymparam.dym_type_speed();

    steer.solve_mid_error();
    steer.update_fuzzy_pid();

    inoutdev.car_servo.set((int)steer.steer_out);
}

void pos_loop_ctrl()
{

    pos_loop.update(chassis.all_distance);
    chassis.set_speed(pos_loop.output, 0);

    // rt_thread_mdelay(20);
}

void task_ctrl(int invers_flag)
{
    dymparam.foresight = 90;

    rt_sem_take(csi_done_sem, RT_WAITING_FOREVER);
    rt_sem_control(csi_done_sem, RT_IPC_CMD_RESET, RT_NULL);

    img_proc_c.img_resize((uint8 **)mt9v03x_csi_image, (uint8 **)gray_120_160_image, 240, 320, 2);
    img_proc_c.gray_to_binary((uint8 **)gray_120_160_image, (uint8 **)bin_120_160_image, 120, 160, 1);

    find_track_gray();

    steer.solve_mid_error();
    steer.update_fuzzy_pid();
    if (chassis.speed_now[0] < 0)
        steer.steer_out = -steer.steer_out;

    inoutdev.car_servo.set((int)steer.steer_out);
}

void pos_loop_loop(int type)
{
    while (1)
    {
        task_ctrl(0);

        if (type == 1)
        {
            // f1.chassis_update_type=0;
            pos_loop.update(chassis.all_distance);
            // chassis.set_speed(pos_loop.output, 0);
            if(fabs(pos_loop.error)>=3)
            {
                chassis.set_speed(pos_loop.output, 0);
                f1.chassis_update_type=0;
            }
            else
            {
                f1.chassis_update_type=3;
                break;
            }


            // if (fabs(chassis.all_distance - pos_loop.target) <= 3
            //     // fabs(chassis.speed_ave) <= 0.4
            // )
            if (((chassis.all_distance >= pos_loop.target - 2) && pos_loop.target > 0) ||
                ((chassis.all_distance <= pos_loop.target + 2) && pos_loop.target < 0) ||
                ((fabs(chassis.all_distance - pos_loop.target) <= 1.5) && pos_loop.target == 0))
            {
                // chassis.start_brake();
                break;
            }
            rt_thread_mdelay(5);
        }

        if (type == 2)
        {
            pos_loop_two.update(chassis.all_distance);

            // if(fabs(pos_loop_two.error)>5)
            //     chassis.set_speed(pos_loop_two.output, 0);
            // else
            //     chassis.set_speed(0, 0);

            if(fabs(pos_loop_two.error)>=3)
            {
                chassis.set_speed(pos_loop_two.output, 0);
                f1.chassis_update_type=0;
            }
            else
            {
                f1.chassis_update_type=3;
                break;
            }

            // if (fabs(chassis.all_distance - pos_loop.target) <= 3
            //     // fabs(chassis.speed_ave) <= 0.4
            // )
            if (((chassis.all_distance >= pos_loop_two.target - 2) && pos_loop_two.target > 0) ||
                ((chassis.all_distance <= pos_loop_two.target + 2) && pos_loop_two.target < 0) ||
                ((fabs(chassis.all_distance - pos_loop_two.target) <= 5) && pos_loop_two.target == 0))
            {
                // chassis.start_brake();
                break;
            }
            rt_thread_mdelay(5);
        }
    }
    chassis.set_speed(0, 0);
}

int main(void)
{
#if 1
    MT9V03X_CFG_CSI[0][1] = f1.cam_auto_exp;   //自动曝光
    MT9V03X_CFG_CSI[1][1] = f1.cam_bright_num; //曝光度
    MT9V03X_CFG_CSI[2][1] = f1.cam_fps;        //帧率
    
    art_pid = cur_pid(1.1, 0.6, 0, 1000, 1000);
    pos_loop = pos_pid(4.0, 0, 90, 100, -100);
    // pos_loop = pos_pid(3.0, 0, 30, 100, -100);
    pos_loop_two = pos_pid(4.0, 0, 55, 120, -120);
    chassis_mutex = rt_mutex_create("chassis_mutex", RT_IPC_FLAG_FIFO);
    csi_done_sem = rt_sem_create("csi_done_sem", 0, RT_IPC_FLAG_FIFO);
    ano_upload_sem = rt_sem_create("ano_upload_sem", 0, RT_IPC_FLAG_FIFO);

    inoutdev.init();
    steer.init();
    ano.init(115200);
    chassis.init();
    init_openart();

    rt_timer_t bat_timer = rt_timer_create("bat_timer", adc_bat_thread, RT_NULL, ADC_BAT_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(bat_timer);

    rt_timer_t gyro_timer = rt_timer_create("gyro_timer", gyro_thread, RT_NULL, GYRO_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(gyro_timer);

    current_timer = rt_timer_create("current_timer", current_thread, RT_NULL, CURRENT_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    chassis.stop();

    rt_timer_t timer = rt_timer_create("chassis_timer", chassis_thread, RT_NULL, CHASSIS_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(timer);

    rt_thread_t tid_display = rt_thread_create("display_thread", display_thread, RT_NULL, 2048, 31, 5);
    rt_thread_startup(tid_display);


#if 1
    while (1)
    {

        rt_sem_take(csi_done_sem, RT_WAITING_FOREVER);
        rt_sem_control(csi_done_sem, RT_IPC_CMD_RESET, RT_NULL);

        img_proc_c.img_resize((uint8 **)mt9v03x_csi_image, (uint8 **)gray_120_160_image, 240, 320, 2);
        img_proc_c.gray_to_binary((uint8 **)gray_120_160_image, (uint8 **)bin_120_160_image, 120, 160, 1);

        inoutdev.clock();

        if (f2.sch_sm_state == normal)
        {
            if (f1.no_ele_start_flag)
            {
                f2.en_start = 1;
                f1.no_ele_start_flag = 0;

                inoutdev.car_servo.set(0);
                rt_timer_stop(current_timer);
                chassis.stop();
                rt_thread_mdelay(2000);
                rt_timer_start(current_timer);
            }

            // if( (pit_get_ms(PIT_CH0)-test_start_time) > 2*126*1000)
            // {
            //     while(1)
            //     {
            //          chassis.set_speed(0, 0);
            //     }
            // }

            normal_run();
            if (f2.stop_car_flag)
                chassis.set_speed(0, 0);
            else
                chassis.set_speed(dymparam.speed_set, (int)steer.steer_out);
        }
        // else if (f2.sch_sm_state == sch_pos_loop)
        // {
        //     pos_loop.update(chassis.all_distance);
        //     chassis.set_speed(pos_loop.output, 0);

        //     if (fabs(chassis.all_distance - pos_loop.target) <= 3 &&
        //         fabs(chassis.speed_ave) <= 1)
        //     {
        //         f2.sch_sm_state = regco;
        //     }
        // }
        else if (f2.sch_sm_state == regco&& f1.start_regco_flag==1)
        {
            
            inoutdev.art_servo.set(1000);
            inoutdev.car_servo.set(0);
            chassis.set_speed(0, 0);

            pos_loop.target = 0;
            pos_loop_loop(1);
            // task_ctrl(0);
            // pos_loop.update(chassis.all_distance);
            // chassis.set_speed(pos_loop.output, 0);

            
            if (f1.finish_regco_flag == 1 && f1.regco_num != -1)
            {
                // static int first_start_regco_flag=0;
                // static int first_start_regco_cnt=0;
                
                // if(first_start_regco_cnt>=3)
                // {
                //     first_start_regco_flag=1;
                // }
                // if(!first_start_regco_flag)
                // {
                //     first_start_regco_cnt++;
                //     continue;
                // }
                
                if (f1.regco_num == 0) //动物，摇摆摄像头
                {
                    // inoutdev.art_servo.set(0);
                    // rt_thread_mdelay(400);
                    // inoutdev.art_servo.set(1000);
                    // rt_thread_mdelay(1000);
                    
                    inoutdev.beep(1);
                    inoutdev.car_servo.set(-1000);
                    rt_thread_mdelay(200);
                    inoutdev.car_servo.set(1000);
                    rt_thread_mdelay(200);
                    inoutdev.car_servo.set(0);

                    rt_thread_mdelay(500);

                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
                else if (f1.regco_num == 1) //水果，打开激光
                {
                    inoutdev.beep(1);
                    inoutdev.laser_on();
                    rt_thread_mdelay(1100);
                    inoutdev.laser_off();
                    rt_thread_mdelay(200);
                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
                else if (f1.regco_num == 2) //偶数，前进30cm再后退
                {
                    inoutdev.beep(1);
                    pos_loop_two.target = 32;
                    pos_loop_loop(2);

                    pos_loop_two.target = 0;
                    pos_loop_loop(2);

                    rt_thread_mdelay(200);
                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
                else if (f1.regco_num == 3) //奇数，后退30cm再前进
                {
                    inoutdev.beep(1);
                    pos_loop_two.target = -32;
                    pos_loop_loop(2);

                    pos_loop_two.target = 0;
                    pos_loop_loop(2);

                    rt_thread_mdelay(200);
                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
                else if (f1.regco_num == 4) //偶数二维码，前进10cm再后退
                {
                    inoutdev.beep(1);
                    pos_loop.target = 12;
                    pos_loop_loop(1);

                    pos_loop.target = 0;
                    pos_loop_loop(1);

                    rt_thread_mdelay(200);
                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
                else if (f1.regco_num == 5) //奇数二维码，后退10cm再前进
                {
                    inoutdev.beep(1);
                    pos_loop.target = -12;
                    pos_loop_loop(1);

                    pos_loop.target = 0;
                    pos_loop_loop(1);

                    rt_thread_mdelay(200);
                    f1.finish_regco_flag = 0;
                    f1.regco_num = -1;
                }
            }
        }
    }
#endif
#endif
}

EX_UPPER_F(set_chassis_pid_param_cmd,
           chassis.L_wheel_kp = atof(argv[1]);
           chassis.L_wheel_ki = atof(argv[2]);
           chassis.L_wheel_kd = atof(argv[3]);
           chassis.R_wheel_kp = atof(argv[4]);
           chassis.R_wheel_ki = atof(argv[5]);
           chassis.R_wheel_kd = atof(argv[6]);
           chassis.left_wheel.w_controller.set_pid(chassis.L_wheel_kp, chassis.L_wheel_ki, chassis.L_wheel_kd);
           chassis.right_wheel.w_controller.set_pid(chassis.R_wheel_kp, chassis.R_wheel_ki, chassis.R_wheel_kd);)

EX_UPPER_F(set_steer_pid_param_cmd,
           steer.steer_basic_kp = atof(argv[1]);
           steer.steer_kp_j = atof(argv[2]);
           //    steer.steer_kp_max = atof(argv[3]);
           steer.steer_ki = atof(argv[4]);
           steer.steer_kd = atof(argv[5]);)

EX_UPPER_F(set_dym_speed_param_cmd,
           dymparam.s_max_speed = atoi(argv[1]);
           dymparam.s_min_speed = atoi(argv[2]);
           dymparam.fore_min = atoi(argv[4]);
           dymparam.fore_max = atoi(argv[5]);
           dymparam.fore_fix = atoi(argv[6]);)

EX_UPPER_F(set_chassis_speed_cmd,
           dymparam.const_speed = atof(argv[1]);
           chassis.set_speed(dymparam.const_speed, 0);)

EX_UPPER_F(set_art_pid_param_cmd,
           art_pid.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));)

EX_UPPER_F(set_cur_pid_param_cmd,
           //    chassis.left_wheel.current_pid.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));
           //    chassis.right_wheel.current_pid.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));
           pos_loop.set_pid(atof(argv[1]), atof(argv[2]), atof(argv[3]));)

EX_UPPER_F(set_cur_pid_target,
           chassis.left_wheel.current_pid.target = atof(argv[1]);
           chassis.right_wheel.current_pid.target = atof(argv[1]);)

EX_UPPER_F(set_steer_fuzzy_p,
           steer.steer_fuzzy_kp_p = atoi(argv[1]);)

EX_UPPER_F(set_remote_stop,
           f1.remote_stop = atoi(argv[1]);
           rt_kprintf("chassis.allele_distance: %f\n", chassis.allele_distance);)

EX_UPPER_F(set_dym_p,
           dymparam.dym_p1 = atof(argv[1]);
           dymparam.dym_p2 = atof(argv[2]);)

//#endif