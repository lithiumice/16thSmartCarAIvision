#ifndef _Findline_H_
#define _Findline_H_

#include "main.h"
#include "stdlib.h"
#include "math.h"

#ifndef CLION_DEBUG
#include "common.h"
#include <rtthread.h>
using namespace rtthread;
#include "img_proc.h"
//#include "ctrllor_class.h"
#include "common_macro.h"
#else
#include "common.h"
#include "img_proc.h"
#include "common_macro.h"
#endif

#define ELE_LOOP 1
#define ELE_FORK 2
#define ELE_QRCODE 3
#define ELE_GARAGE 4
#define ELE_START 5
#define ELE_RAMP 6
#define ELE_L_LOOP 7
#define ELE_R_LOOP 8

#define GRAY_SPAN 3
#define GRAY_THRESHOLD 35 //max 70

#define CAM_AUTO_EXP 30 //26
#define CAM_BRIGHT_NUM 240

typedef struct
{
  rt_tick_t fork_regco_ed_time;
  rt_tick_t qrcode_regco_time;
  rt_tick_t friut_regco_ed_time;
  rt_tick_t laser_cal_time;

  int fork_temp_match_num = -1;
  int current_element_pos = 0;
  int remote_stop = 0;

  int16 reset_ele_signal = 0;
  int16 current_turns = 0;
  int16 chassis_update_type = 0;

  uint32 friut_start_time;
  uint32 start_time1;
  uint32 end_time1;
  uint32 use_time1;

  int gray_span = GRAY_SPAN;
  int gray_threshold = GRAY_THRESHOLD;
  int cam_bright_num = CAM_BRIGHT_NUM;
  int cam_auto_exp = CAM_AUTO_EXP;
  int cam_fps = 100;

  int midline_infec_flag = -1;
  int start_flag = 0;
  int no_ele_start_flag = 0;
  int cam_fps_div = 1;
  int cam_frame_cnt = 0;

  //使能变量
  int en_dis_bake = 1;
  int en_dis_ele = 1;
  int ele_enable = 1;
  int servo_enable = 1;
  int dymspeed_enable = 1;
  int16 en_element_list = 0;
  int16 en_dym_foresight = 1;
  int16 en_chassis_debug = 1;
  int16 en_chassis_update = 1;
  int16 en_stop_car = 1;
  int16 en_ano_upload = 0;

  int16 en_ramp_det = 1;
  int16 en_fork_regco = 1;
  int16 en_anifriut_regco = 1;

  //距离变量
  int16 ano_type = 5;
  int16 disp_pic_type = 0;
  float tag_start_dis = 0;
  float fork_start_dis = 0;
  float bat_voltage = 0;
  float garage_regco_dis = 0;
  float L_loop_start_dis = 0;
  float R_loop_start_dis = 0;

  //art相关
  int rev_type = 0;
  int rev_num = 0;
  int rev_conf = 0;
  int rev_ed_msg = 0;

  int fork_regco_num = 0;
  int aptg_regco_num = 0;
  int anifruit_regco_num = 0;
  int fruit_pos_num = 0;
  int art_servo_comp = 0;

  int aptg_regco_num_reg1 = 0;
  int anifruit_regco_num_reg1 = 0;
  int aptg_regco_num_reg2 = 0;
  int anifruit_regco_num_reg2 = 0;

  int rev_ok_flag = 0;
  int finish_regco_flag = 0;
  int rev_num_list[10] = {-1};
  int friut_pos_list[10] = {-1};

  //赛道使能
  int16 garage_enable = 1;
  int16 loop_enable = 1;
  int16 cross_enable = 1;
  int16 fork_enable = 1;
  int16 curve_enable = 1;
  int16 tag_enable = 1;
  int16 ramp_enable = 1;
  int16 start_enable = 1;
  int16 s_cross_enable = 0;

  int16 midline[120];
  int16 leftline[120];
  int16 rightline[120];
  int16 leftlineflag[120];
  int16 rightlineflag[120];
  int16 real_width[120];
  int16 mid_change[120];

  int16 midline_infec_row;
  int16 leftstartrow;
  int16 rightstartrow;

  int16 tag_toppoint;
  int16 tag_L_start_row;
  int16 tag_R_start_row;
  int16 road_real_width[120];
  int16 tag_black_duty[120];
  int16 tag_leftline[120];
  int16 tag_leftlineflag[120];
  int16 tag_rightline[120];
  int16 tag_rightlineflag[120];
  int16 cross_down_line[160];
  int16 cross_down_line_flag[160];

} Findline1_TypeDef;
extern Findline1_TypeDef f1;

typedef struct
{
  int cross_start_row;
  int s_cross_flag;

  int fork1_dir = 0;
  int fork2_dir = 0;

  //灰度
  int16 Image_gray_row[120][160];

  //状态机
  int16 tag_sm_state = 0;
  int16 tag_proc_sm = 0;

  int16 L_loop_judge_sm = 0;
  int16 R_loop_judge_sm = 0;
  int L_loop_sm_state;
  int R_loop_sm_state;

  int fork_proc_sm = 0;
  int sch_sm_state = 0;
  int16 fork_sm_state = 0;

  // int16 fork_dir = 1;
  int16 fork_frames = 0;
  int fork_num = 0;

  //坡道
  int16 ramp_sm_state = 0;
  int16 ramp_flag = 0;
  int16 ramp_frames = 0;
  int16 ramp_top_row = 0;

  //出库
  int16 en_start = 0;
  int16 start_sm_state = 0;
  int start_num = 0;

  //停车
  int16 stop_car_reg[20];
  int16 stop_car_flag;

  //弯道
  int16 leftturnflag;
  int16 rightturnflag;
  int16 leftturn_row;
  int16 rightturn_row;

  //二维码
  int qrcode_num = 0;
  int16 tag_frames;
  int16 tag_flag;
  int16 duty_threshold = 15;

  int16 tag_top_row;
  int16 tag_row_cnt;
  int16 tag_row_up;
  int16 tag_row_down;
  int16 tag_row_flag;
  int qrcode_row_reg;

  int16 tag_left_col;
  int16 tag_right_col;

  int16 tag_left[2];
  int16 tag_right[2];

  int16 L_col1 = 0;
  int16 L_col2 = 0;
  int16 R_col1 = 0;
  int16 R_col2 = 0;

  //入库
  int16 garage_tmp[2] = {0};
  int16 garage_block_cnt = 0;
  int16 garage_row = 0;
  int16 garage_frames = 0;

  int16 garage_dir = 0;
  int16 garage_flag = 0;
  int16 garage_sm_state = 0;
  int16 garage_start_row = 0;

  //拐点
  int16 L_infec_col;
  int16 L_infec_row;
  int16 L_infec_flag;

  int16 R_infec_flag;
  int16 R_infec_col;
  int16 R_infec_row;

  int16 L_up_infec_col;
  int16 L_up_infec_row;
  int16 L_up_infec_flag;

  int16 R_up_infec_flag;
  int16 R_up_infec_col;
  int16 R_up_infec_row;

  //基础巡线
  int16 toppoint;
  int16 toprow_80col;
  int16 toppoint_reg;
  int16 toppoint_flag;
  int16 column_start = 80;

  //十字
  int16 L_mid_endrow = 0;
  int16 R_mid_endrow = 0;
  int16 cross_sm_state = 0;
  int16 cross_flag;
  int16 cross_frames;
  int16 cross_type;
  float cross_K_L;
  float cross_K_R;
  float midline_K;

  //左环岛
  int loop_num = 0;
  int L_loop_flag;

  int L_loop_circle_row;
  int L_loop_circle_col;
  int L_loop_circle_flag;

  int L_loop_top_row;
  int L_loop_up_row;
  int L_loop_down_row;
  int16 L_loop_Redge_endrow = 0;
  int16 L_loop_up_infec_row = 0;

  int16 L_loop_exit_row;
  int16 L_loop_exit_col;
  int16 L_loop_exit_flag;

  //右环岛
  int16 R_loop_flag;

  int16 R_loop_circle_row;
  int16 R_loop_circle_col;
  int16 R_loop_circle_flag;

  int16 R_loop_top_row;
  int16 R_loop_up_row;
  int16 R_loop_down_row;
  int16 R_loop_Ledge_endrow = 0;
  int16 R_loop_up_infec_row = 0;

  int16 R_loop_exit_row;
  int16 R_loop_exit_col;
  int16 R_loop_exit_flag;

  //三岔路
  int16 fork_flag;
  int16 fork_L_flag;
  int16 fork_L_col;
  int16 fork_L_row;
  int16 fork_R_flag;
  int16 fork_R_col;
  int16 fork_R_row;
  int16 fork_up_flag;
  int16 fork_up_col;
  int16 fork_up_row;

  int16 fork_up_L_col;
  int16 fork_up_R_col;

  int16 fork_up_line[160];
  int16 fork_up_line_flag[160];

  int16 R_fork_mid_endrow = 0;
  int16 L_fork_mid_endrow = 0;

  //长直道
  int16 shortstraight_flag;
  int16 crossstraight_flag;
  int16 long_straight_flag;

} Findline2_TypeDef;
extern Findline2_TypeDef f2;

void find_track_gray();
void find_toppoint();
void find_little_curve();
void find_large_curve();
void find_cross_infec();
void find_L_loop_exit();
void qrcode_judge();
void cross_judge();
void find_track_bin();
void find_elements();
void find_L_loop_circle();
void long_judge();
void short_judge();
void stop_judge();
void find_up_infec();
void find_fork_up_row();
void find_qrcode_pos();
void find_qrcode_row();
void enable_all_elements();
void find_L_edge_from_outter();
void find_R_edge_from_outter();
void find_topstart_row();
void find_road_duty();
void find_down_line();
void continue_patch();
void find_mem_R_edge();
void find_mem_L_edge();
void find_infec();
void find_L_egde_from_R();
void find_R_tag_toppoint();
void find_R_egde_from_L();
void find_L_tag_toppoint();
void L_mid_endrow_proc();
void R_mid_endrow_proc();
void L_loop_judge();
void R_loop_judge();
void L_loop_patch();
void R_loop_patch();
void L_fork_patch();
void R_fork_patch();
void fork_road_judge();
void qrcode_patch();
void qrcode_judge();
void continue_patch();
void garage_judge();
void cross_judge();
void cross_patch();
void ramp_judge();
void curve_patch();

float solve_variance(int begin_row, int end_row);
float solve_slope(int begin, int end, int16 *p);
int solve_region_duty(int x1, int y1, int x2, int y2, int color);
int is_continuous_line(int start, int end, int16 *src_line, int threshold);
int is_L_R_lose(int start, int end, int mode, int threshold);
int is_L_R_nonlose(int start, int end, int mode, int threshold);
int is_region_one_color(int x1, int y1, int x2, int y2, int color, int threshold);
int is_straight_line(int start_index, int end_index, int16 *line_, int scale);
int is_M_center(int start, int end, int half_err);

extern int16 half_width[120];
extern int16 loop_half_width[120];

#endif