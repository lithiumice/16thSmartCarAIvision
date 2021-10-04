#include "display_ui.h"
#include "main.h"
using namespace rtthread;

int16 refresh_lcd_signal = 0;
//int64 test_start_time;
std::string ui_type;
ui_modify_x x_1;
ui_option_t ui_option;
void get_half_width();

ui_title_x ui_title_y[] = {
    {show_image_i, "show image", show_image, 0},
    {show_param_i, "show param", ui_param_fresh, 0},

    {tweak_bright_i, "tweak_bright", tweak_bright, 0},
    {tweak_auto_exp_i, "tweak_auto_exp", tweak_auto_exp, 0},
    {tweak_threshold_i, "tweak_threshold", tweak_threshold, 0},

    {change_param_i, "change param", ui_modify_fresh, 0},
    {line_pos_i, "line_pos", show_line, 0},

    {10, "get_half_width", get_half_width, 1},
    {10, "upload_zf_bin", upload_zf_bin, 1},
    {10, "upload_zf_gray", upload_zf_gray, 1},
};

ui_modify_x ui_modify_y[] = {
    {0, "en_dym_foresight", &f1.en_dym_foresight, "int16", 0, 1, 1},
    {0, "current_turns", &f1.current_turns, "int16", 0, 1, 1},
    {0, "f1.ano_type", &f1.ano_type, "int16", 0, 1, 3},
    {0, "disp_pic_type", &f1.disp_pic_type, "int16", 0, 1, 2},
};

ui_param_x ui_param_y[] = {
    PU(all_distance, chassis.all_distance, float),
    PU(cross_z_angle, inoutdev.cross_z_angle, float),
    PU(gyro_z_angle, inoutdev.gyro_z_angle, float),

    PU(roll_angle, inoutdev.roll_angle, float),
    PU(yaw_angle, inoutdev.yaw_angle, float),
    PU(pitch_angle, inoutdev.pitch_angle, float),
    
    PU(acc_angle, inoutdev.acc_angle, float),
    PU(gyro_y_omega, inoutdev.gyro_y_omega, float),
    PU(pitch_rate, inoutdev.pitch_rate, float),
    PU(yaw_rate, inoutdev.yaw_rate, float),
    PU(true_yaw_rate, inoutdev.true_yaw_rate, float),

    PU_S(steer.steer_out, float),
    PU_S(f1.ps2_key, uint8),
    PU_S(f1.remote_stop, int),
    PU(s1, inoutdev.remote1_start_time, uint64),
    PU(e1, inoutdev.remote1_val, uint64),
    PU(s2, inoutdev.remote2_start_time, uint64),
    PU(e2, inoutdev.remote2_val, uint64),

    PU_S(icm_acc_x, int16),
    PU_S(icm_acc_y, int16),
    PU_S(icm_acc_z, int16),
    PU_S(icm_gyro_x, int16),
    PU_S(icm_gyro_y, int16),
    PU_S(icm_gyro_z, int16),

    PU_S(f2.tag_proc_sm, int16),
    PU_S(f2.fork_proc_sm, int),
    PU_S(f2.sch_sm_state, int),
    PU_S(f2.L_loop_sm_state, int),
    PU_S(f2.R_loop_sm_state, int),
    PU_S(f2.fork_sm_state, int16),
    PU_S(f2.ramp_sm_state, int16),
    PU_S(f2.garage_sm_state, int16),

    PU_S(f1.en_dis_ele, int),
    PU_S(f1.en_dis_bake, int),
    PU_S(f1.dymspeed_enable, int),

    PU_S(f2.cross_K_L, float),
    PU_S(f2.cross_K_R, float),
    PU_S(f2.L_infec_row, int16),
    PU_S(f2.L_infec_col, int16),

    PU_S(f1.fork_enable, int16),
    PU_S(f1.loop_enable, int16),
    PU_S(f1.ramp_enable, int16),
    PU_S(f1.garage_enable, int16),
    PU_S(f1.tag_enable, int16),

    {0, "f2.fork_num", &f2.fork_num, "int"},
    {0, "qrcode_num", &f2.qrcode_num, "int"},
    {0, "f2.loop_num", &f2.loop_num, "int"},
    {0, "f2.start_num", &f2.start_num, "int"},

    {0, "f2.toppoint", &f2.toppoint, "int16"},
    {0, "f2.toprow_80col", &f2.toprow_80col, "int16"},
    {0, "f2.ramp_top_row", &f2.ramp_top_row, "int16"},

    {0, "f1.real_width[85],", &f1.real_width[85], "int16"},
    {0, "f1.real_width[90],", &f1.real_width[90], "int16"},
    {0, "f1.real_width[95],", &f1.real_width[95], "int16"},
    {0, "f1.real_width[100]", &f1.real_width[100], "int16"},
    {0, "f1.real_width[105]", &f1.real_width[105], "int16"},
    {0, "f1.real_width[110]", &f1.real_width[110], "int16"},
    {0, "img_threshold", &img_proc_c.img_threshold, "int16"},
    {0, "fork_temp_match_num", &f1.fork_temp_match_num, "int"},
    {0, "current_element_pos", &f1.current_element_pos, "int16"},
    {0, "speed_set", &dymparam.speed_set, "float"},
    {0, "mid error", &steer.img_err_ave, "int16"},
    {0, "f1.bat_voltage", &f1.bat_voltage, "float"},

    {0, "all distance", &chassis.all_distance, "float"},
    {0, "loop distance", &chassis.loop_distance, "float"},
    {0, "fork_distance", &chassis.fork_distance, "float"},
    {0, "start_distance", &chassis.start_distance, "float"},
    {0, "tag_distance", &chassis.tag_distance, "float"},

    {0, "right_adc", &chassis.right_wheel.current_adc, "float"},
    {0, "right_inc", &chassis.right_wheel.current_pid.output, "float"},

    {0, "left_adc", &chassis.left_wheel.current_adc, "float"},
    {0, "left_inc", &chassis.left_wheel.current_pid.output, "float"},

    {0, "speed_set", &dymparam.speed_set, "float"},

    {0, "cam fps", &inoutdev.fps, "int"},
    {0, "foresight", &dymparam.foresight, "int16"},
    {0, "fore_fix", &dymparam.fore_fix, "int16"},
    {0, "fore_max", &dymparam.fore_max, "int16"},
    {0, "fore_min", &dymparam.fore_min, "int16"},
    {0, "const speed", &dymparam.const_speed, "float"},

    {0, "steer kp", &steer.steer_kp, "float"},
    {0, "steer kd", &steer.steer_kd, "float"},

};

void display_thread(void *param)
{
    while (1)
    {
        ui_title_fresh(NULL);
        rt_thread_mdelay(100);
    }
}

void ui_title_fresh(void *parameter)
{
    if (refresh_lcd_signal)
    {
        lcd_clear(WHITE);
        refresh_lcd_signal = 0;
    }

    if (0 == ui_option.level)
    {
        int ui_index = 0;
        int start_pos = UI_LIST_MAX_LEN * ui_option.current_title_page;
        int end_pos = (UI_TITLE_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_TITLE_LEN - start_pos);

        for (int i = start_pos; i < start_pos + end_pos; i++)
        {
            ui_title_x x_1 = ui_title_y[i];
            std::string ui_str = "   " + (std::string)x_1.title;

            if (ui_index == (ui_option.cursor_level0 % UI_LIST_MAX_LEN))
            {
                ui_str[1] = '>';
                lcd_flexstr(0, ui_index, RED, WHITE, ui_str.data(), 0);
            }
            else
            {
                ui_str[1] = ' ';
                lcd_flexstr(0, ui_index, BLACK, WHITE, ui_str.data(), 0);
            }
            ui_index++;
        }
    }
    else if (1 == ui_option.level)
    {
        (*(ui_title_y[ui_option.cursor_level0].current_func))();
    }
}

void ui_param_fresh()
{
    int ui_index = 0;
    int start_pos = UI_LIST_MAX_LEN * ui_option.current_param_page;
    int end_pos = (UI_PARAM_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_PARAM_LEN - start_pos);

    for (int i = start_pos; i < start_pos + end_pos; i++)
    {
        ui_param_x x_1 = ui_param_y[i];
        std::string ui_str = x_1.param_title;
        std::string ui_type = x_1.param_type;

        if (ui_type == "float")
        {
            TS(ui_index, float, x_1.param_addr);
        }
        else if (ui_type == "int")
        {
            TS(ui_index, int, x_1.param_addr);
        }
        else if (ui_type == "int16")
        {
            TS(ui_index, int16, x_1.param_addr);
        }
        else if (ui_type == "uint8")
        {
            TS(ui_index, uint8, x_1.param_addr);
        }
        else if (ui_type == "uint64")
        {
            TS(ui_index, uint64, x_1.param_addr);
        }

        ui_index++;
    }
}

void ui_modify_fresh()
{
    if (refresh_lcd_signal)
    {
        lcd_clear(WHITE);
        refresh_lcd_signal = 0;
    }

    int ui_index = 0;
    int start_pos = UI_LIST_MAX_LEN * ui_option.current_modify_page;
    int end_pos = (UI_MODIFY_LEN - start_pos) > UI_LIST_MAX_LEN ? UI_LIST_MAX_LEN : (UI_MODIFY_LEN - start_pos);

    for (int i = start_pos; i < start_pos + end_pos; i++)
    {
        ui_modify_x x_1 = ui_modify_y[i];
        std::string ui_name = x_1.num_name;
        std::string ui_type = x_1.num_type;
        std::string ui_str;

        if (ui_type == "int16")
        {
            int16 a = *(int16 *)(x_1.num_addr);
            ui_str = "  " + ("(" + std::to_string(a) + ")") + ui_name;
        }

        if (ui_index == (ui_option.cursor_level1 % UI_LIST_MAX_LEN))
        {
            ui_str[1] = '>';
            lcd_flexstr(0, ui_index, RED, WHITE, ui_str.data(), 0);
        }
        else
        {
            ui_str[1] = ' ';
            lcd_flexstr(0, ui_index, BLACK, WHITE, ui_str.data(), 0);
        }

        ui_index++;
    }
}

void send_reset_sig()
{
    f1.reset_ele_signal = 1;

    f1.current_turns = 0;
    f1.en_element_list = 0;
    f1.current_element_pos = 0;
    f1.chassis_update_type = 0;

    chassis.clear_dis();

    inoutdev.gyro_z_angle = 0;
    inoutdev.gyro_y_angle = 0;
    inoutdev.start_angle = 0;

    CLR_ARG(dymparam.road_type);
    inoutdev.art_servo.set(0);
    f1.start_regco_flag=1;
    // f1.game_start_time=pit_get_ms(PIT_CH0);
}

void show_image()
{
    show_cam();

    for (int i = f2.toppoint; i < 120; i++)
    {
        lcd_drawpoint(f1.midline[i], i, BLUE);
        lcd_drawpoint(f1.leftline[i], i, RED);
        lcd_drawpoint(f1.rightline[i], i, RED);
    }

    lcd_drawline(0, dymparam.foresight, 159, dymparam.foresight, YELLOW);
    gui_circle(steer.img_err_ave + 80, 116, RED, 2, 1);

    if (f2.fork_sm_state != 0)
    {
        gui_circle(f2.fork_L_col, f2.fork_L_row, BLUE, 2, 1);
        gui_circle(f2.fork_R_col, f2.fork_R_row, BLUE, 2, 1);
        gui_circle(f2.fork_up_col, f2.fork_up_row, BLUE, 2, 1);
    }

    ui_option.ui_detail_index = 0;
    FS(fps, inoutdev.fps);
    FS(sch, f2.sch_sm_state);
    FS(Lstart, f1.leftstartrow);
    FS(Rstart, f1.rightstartrow);
    lcd_flexstr(0, ui_option.ui_detail_index++, RED, WHITE, ("road: " + dymparam.road_name[dymparam.road_type[0]]).data(), 1);
    lcd_flexstr(0, ui_option.ui_detail_index++, RED, WHITE, ("loop: " + std::to_string(f2.L_loop_sm_state) + "," + std::to_string(f2.R_loop_sm_state)).data(), 1);
}

void show_cam()
{
    if (f1.disp_pic_type == 0)
        lcd_displayimage032(bin_120_160_image[0], 160, 120);
    else if (f1.disp_pic_type == 1)
        lcd_displayimage032(gray_120_160_image[0], 160, 120);
}

void tweak_bright()
{
    show_cam();
    ui_option.ui_detail_index = 0;
    FS(bright, f1.cam_bright_num);
}

void tweak_auto_exp()
{
    show_cam();

    ui_option.ui_detail_index = 0;
    FS(auto_exp, f1.cam_auto_exp);
}

void tweak_threshold()
{
    show_cam();

    ui_option.ui_detail_index = 0;
    FS(threshold, img_proc_c.fix_threshold);
}

void show_line()
{
    show_cam();

    lcd_drawline(0, ui_option.line_pos, 159, ui_option.line_pos, YELLOW);
    lcd_flexstr(0, 0, RED, WHITE, ("pos: " + std::to_string(ui_option.line_pos)).data(), 1);
}

void btn_event_cb(agile_btn_t *btn)
{
    inoutdev.beep(1);
    refresh_lcd_signal = 1;
    switch (btn->pin)
    {
    case KEY6_PIN: //down
        if (0 == ui_option.level)
        {
            __ValuePlus(ui_option.cursor_level0, 1, 0, UI_TITLE_LEN - 1);
            ui_option.current_title_page = ui_option.cursor_level0 / UI_LIST_MAX_LEN;
        }
        else if (1 == ui_option.level)
        {
            switch (ui_option.cursor_level0)
            {
            case show_image_i:
                __ValuePlus(f1.disp_pic_type, 1, 0, 2);
                break;
            case show_param_i:
                __ValuePlus(ui_option.current_param_page, 1, 0, UI_PARAM_MAX_PAGE - 1);
                break;
            case change_param_i:
                __ValuePlus(ui_option.cursor_level1, 1, 0, UI_MODIFY_LEN - 1);
                ui_option.current_modify_page = ui_option.cursor_level1 / UI_LIST_MAX_LEN;
                break;
            case line_pos_i:
                __ValuePlus(ui_option.line_pos, -3, 0, 120);
                break;
            case tweak_bright_i:
                __ValuePlus(f1.disp_pic_type, 1, 0, 2);
                break;
            }
        }
        break;
    case KEY4_PIN: //up
        if (0 == ui_option.level)
        {
            __ValuePlus(ui_option.cursor_level0, -1, 0, UI_TITLE_LEN - 1);
            ui_option.current_title_page = ui_option.cursor_level0 / UI_LIST_MAX_LEN;
        }
        else if (1 == ui_option.level)
        {
            switch (ui_option.cursor_level0)
            {
            case show_image_i:
                __ValuePlus(f1.disp_pic_type, -1, 0, 2);
                break;
            case show_param_i:
                __ValuePlus(ui_option.current_param_page, -1, 0, UI_PARAM_MAX_PAGE - 1);
                break;
            case change_param_i:
                __ValuePlus(ui_option.cursor_level1, -1, 0, UI_MODIFY_LEN - 1);
                ui_option.current_modify_page = ui_option.cursor_level1 / UI_LIST_MAX_LEN;
                break;
            case line_pos_i:
                __ValuePlus(ui_option.line_pos, 3, 0, 120);
                break;
            case tweak_bright_i:
                __ValuePlus(f1.disp_pic_type, -1, 0, 2);
                break;
            }
        }
        break;
    case KEY5_PIN: //+
        if (1 == ui_option.level)
        {
            switch (ui_option.cursor_level0)
            {
            case change_param_i:
                x_1 = ui_modify_y[ui_option.cursor_level1];
                ui_type = x_1.num_type;
                if (ui_type == "int16")
                    P_1(*(int16 *)(x_1.num_addr), x_1.end);
                break;
            case show_image_i:
            case tweak_bright_i:
                __ValuePlus(f1.cam_bright_num, 10, 10, 600);
                MT9V03X_CFG_CSI[1][1] = f1.cam_bright_num;
                mt9v03x_csi_init();
                break;
            case tweak_threshold_i:
                __ValuePlus(img_proc_c.fix_threshold, 5, 5, 600);
                break;
            case tweak_auto_exp_i:
                __ValuePlus(f1.cam_auto_exp, 1, 0, 63);
                MT9V03X_CFG_CSI[0][1] = f1.cam_auto_exp;
                mt9v03x_csi_init();
                break;
            }
        }
        break;
    case KEY7_PIN: //-
        if (1 == ui_option.level)
        {
            switch (ui_option.cursor_level0)
            {
            case change_param_i:
                x_1 = ui_modify_y[ui_option.cursor_level1];
                ui_type = x_1.num_type;
                if (ui_type == "int16")
                    M_1(*(int16 *)(x_1.num_addr), x_1.end);
                break;
            case show_image_i:
            case tweak_bright_i:
                __ValuePlus(f1.cam_bright_num, -10, 10, 600);
                MT9V03X_CFG_CSI[1][1] = f1.cam_bright_num;
                mt9v03x_csi_init();
                break;
            case tweak_threshold_i:
                __ValuePlus(img_proc_c.fix_threshold, -5, 5, 600);
                break;
            case tweak_auto_exp_i:
                __ValuePlus(f1.cam_auto_exp, -1, 0, 63);
                MT9V03X_CFG_CSI[0][1] = f1.cam_auto_exp;
                mt9v03x_csi_init();
                break;
            }
        }

        break;
    case KEY1_PIN: //START
        f1.no_ele_start_flag = 1;
        break;
    case KEY2_PIN: //RESET
        send_reset_sig();
        rt_timer_start(current_timer);
        break;
    case KEY3_PIN: //ENTER
        if (0 == ui_option.level)
        {
            if (0 == ui_title_y[ui_option.cursor_level0].func_type)
                ui_option.level = 1;
            else
                (*(ui_title_y[ui_option.cursor_level0].current_func))();
        }
        else if (1 == ui_option.level)
            ui_option.level = 0;

        break;
    default:
        break;
    }
}
