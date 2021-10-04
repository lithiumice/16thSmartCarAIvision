#include "f_elems.h"
#include "cstdlib"
#include "cstring"
Findline1_TypeDef f1;
Findline2_TypeDef f2;
// 0, 0, 1, 1, 2, 2, 3, 3, 3, 4,
//      4, 5, 5, 5, 6, 6, 7, 7, 8, 8,
//      9, 9, 9, 10, 10, 11, 11, 12, 12, 12,13,
// 14,15,16,17,18,19,20,21,21,22,
// 23,24,24,25,26,27,27,28,29,29,
// 30,31,33,37,35,34,34,35,36,36,
// 37,38,38,39,40,41,41,42,43,44,
// 44,45,46,47,47,48,49,49,50,51,
// 52,52,53,54,55,55,56,57,57,58,
// 59,59,60,61,62,62,63,64,64,65,
// 66,66,67,68,68,69,70,71,71,72,
// 73,74,74,75,76,76,76,77,77,
int16 half_width[120] =
    {0, 0, 1, 1, 2, 2, 3, 3, 3, 4,
     4, 5, 5, 5, 6, 6, 7, 7, 8, 8,
     9, 9, 9, 10, 10, 11, 11, 12, 12, 12,13,
14,15,16,17,18,19,20,21,21,22,
23,24,24,25,26,27,27,28,29,29,
30,31,33,37,35,34,34,35,36,36,
37,38,38,39,40,41,41,42,43,44,
44,45,46,47,47,48,49,49,50,51,
52,52,53,54,55,55,56,57,57,58,
59,59,60,61,62,62,63,64,64,65,
66,66,67,68,68,69,70,71,71,72,
73,74,74,75,76,76,76,77,77};


int16 loop_half_width[120] =
    {22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 25,
     28, 31, 33, 35, 36, 37, 39, 41, 42, 44,
     45, 44, 44, 43, 43, 43, 43, 43, 43, 43,
     43, 43, 43, 43, 46, 46, 47, 48, 48, 48,
     49, 50, 50, 50, 50, 51, 51, 52, 52, 53,
     53, 53, 54, 54, 55, 55, 55, 56, 57, 57,
     58, 58, 59, 60, 60, 61, 61, 62, 63, 64,
     64, 64, 65, 67, 67, 67, 68, 68, 68, 68,
     68, 68, 68, 68, 69, 69, 69, 70, 70, 71};

void enable_all_elements()
{
  f1.garage_enable = 1;
  f1.fork_enable = 1;
  f1.loop_enable = 1;
  f1.tag_enable = 1;
  f1.ramp_enable = 1;
  f1.start_enable = 1;

  f1.curve_enable = 1;
  f1.cross_enable = 1;
  f1.s_cross_enable = 1;
}

void disable_all_elements()
{
  f1.garage_enable = 0;
  f1.fork_enable = 0;
  f1.loop_enable = 0;
  f1.tag_enable = 0;
  f1.ramp_enable = 0;
  f1.start_enable = 0;

  f1.curve_enable = 0;
  f1.cross_enable = 0;
  f1.s_cross_enable = 0;
}

void find_post_proc()
{
  find_toppoint();
  long_judge();
  short_judge();
  stop_judge();

  f2.column_start = f1.midline[118];
  __Limit_AB(f2.column_start, 10, 150);
}

void find_qrcode_pos()
{
  f2.qrcode_row_reg = f2.tag_row_down;
  f2.tag_row_down = 0;
  find_topstart_row();
  find_road_duty();

  for (int i = 115; i > f2.tag_top_row; i--)
  {
    if (
        (f1.tag_black_duty[i + 1] < f2.duty_threshold) &&

        f1.tag_black_duty[i + 0] > f2.duty_threshold &&
        f1.tag_black_duty[i - 1] > f2.duty_threshold &&
        f1.tag_black_duty[i - 2] > f2.duty_threshold &&
        f1.tag_black_duty[i - 3] > f2.duty_threshold &&

        f1.tag_black_duty[i + 0] < 70 &&
        f1.tag_black_duty[i - 1] < 70 &&
        f1.tag_black_duty[i - 2] < 70 &&
        f1.tag_black_duty[i - 3] < 70)
    {
      f2.tag_row_down = i;
      f2.tag_row_flag = 1;
      break;
    }
  }

  if (abs(f2.tag_row_down - f2.qrcode_row_reg) > 30)
  {
    f2.tag_row_down = f2.qrcode_row_reg;
    f2.tag_row_flag = 0;
  }

  if (!f2.tag_row_down)
  {
    f2.tag_row_down = f2.qrcode_row_reg;
    f2.tag_row_flag = 0;
  }

  if (is_region_one_color(50, 100, 110, 118, 0, 40))
  {
    f2.tag_row_down = 118;
    f2.tag_row_flag = 0;
  }
}

void find_up_infec()
{
  f2.fork_L_flag = 0;
  f2.fork_R_flag = 0;

  f2.fork_L_col = 0;
  f2.fork_R_col = 159;

  f2.fork_R_row = 118;
  f2.fork_L_row = 118;

  for (int i = 110; i >= 30; i--)
  {
    if (
        f1.leftline[i] < 100 &&
        f1.leftline[i] - f1.leftline[i - 3] >= 1 &&
        f1.leftline[i] - f1.leftline[i + 3] >= 1 &&
        f1.leftline[i + 3] - f1.leftline[i + 6] >= 1 &&
        is_L_R_nonlose(i - 2, i + 4, 0, 100))
    {
      f2.fork_L_flag = 1;
      f2.fork_L_row = i;
      f2.fork_L_col = f1.leftline[i];
      break;
    }
  }

  for (int i = 110; i >= 30; i--)
  {
    if (
        f1.rightline[i] > 60 &&
        f1.rightline[i - 3] - f1.rightline[i] >= 1 &&
        f1.rightline[i + 3] - f1.rightline[i] >= 1 &&
        f1.rightline[i + 6] - f1.rightline[i + 3] >= 1 &&
        is_L_R_nonlose(i - 2, i + 4, 1, 100))
    {
      f2.fork_R_flag = 1;
      f2.fork_R_row = i;
      f2.fork_R_col = f1.rightline[i];
      break;
    }
  }

  memset(f2.fork_up_line, 0, sizeof(f2.fork_up_line));
  memset(f2.fork_up_line_flag, 0, sizeof(f2.fork_up_line_flag));
  for (int i = 0; i < 160; i++)
  // for (int i = f2.fork_L_col; i < f2.fork_R_col; i++)
  {
    for (int j = MIN(f2.fork_R_row, f2.fork_L_row)-5; j > 5; j--)
    {
      if (
          IMG_120_160[j + 1][i] == 0 &&
          IMG_120_160[j][i] != 0)
      {
        break;
      }

      if (
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f2.fork_up_line[i] = j - 1;
        f2.fork_up_line_flag[i] = 1;
        break;
      }
    }
  }

  f2.fork_up_flag = 0;
  f2.fork_up_row = 0;
  f2.fork_up_col = 80;

  for (int i = 20; i < 140; i++)
  {
    if (
        //��
        (f2.fork_up_line[i] >= f2.fork_up_line[i + 2] &&
         f2.fork_up_line[i] >= f2.fork_up_line[i - 2] &&

         f2.fork_up_line[i] > f2.fork_up_line[i + 10] &&
         f2.fork_up_line[i + 10] >= f2.fork_up_line[i + 20] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 10] &&
         f2.fork_up_line[i - 10] >= f2.fork_up_line[i - 20] &&

         f2.fork_up_line_flag[i] &&
         f2.fork_up_line_flag[i + 10] &&
         f2.fork_up_line_flag[i + 20] &&
         f2.fork_up_line_flag[i - 10] &&
         f2.fork_up_line_flag[i - 20] &&

         abs(f2.fork_up_line[i - 10] - f2.fork_up_line[i + 10]) <= 6 &&
         abs(f2.fork_up_line[i - 20] - f2.fork_up_line[i + 20]) <= 6

         ) ||

        //��
        (f2.fork_up_line[i] >= f2.fork_up_line[i + 2] &&
         f2.fork_up_line[i] >= f2.fork_up_line[i - 2] &&

         f2.fork_up_line[i] > f2.fork_up_line[i + 8] &&
         f2.fork_up_line[i + 8] >= f2.fork_up_line[i + 12] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 8] &&
         f2.fork_up_line[i - 8] >= f2.fork_up_line[i - 12] &&

         f2.fork_up_line_flag[i] &&
         f2.fork_up_line_flag[i + 8] &&
         f2.fork_up_line_flag[i + 12] &&
         f2.fork_up_line_flag[i - 8] &&
         f2.fork_up_line_flag[i - 12] &&

         abs(f2.fork_up_line[i - 8] - f2.fork_up_line[i + 8]) <= 6 &&
         abs(f2.fork_up_line[i - 12] - f2.fork_up_line[i + 12]) <= 6))
    {
      f2.fork_up_flag = 1;
      f2.fork_up_col = i;
      f2.fork_up_row = f2.fork_up_line[i];
      break;
    }
  }

  if (f2.fork_up_line[f2.fork_up_col] < 30)
    f2.fork_up_flag = 0;
}

void find_toppoint()
{
  for (int i = 115; i > 5; i--)
  {
    if (
        ((IMG_120_160[i][f1.midline[i]] != 0) &&
         (IMG_120_160[i - 1][f1.midline[i]] == 0 ||
          IMG_120_160[i - 2][f1.midline[i]] == 0)) ||
        (f1.midline[i] >= 159 || f1.midline[i] <= 0))
    {
      f2.toppoint = i;
      f2.toppoint_flag = 1;
      break;
    }
  }
  f2.toppoint_flag = 1;
  __Limit_AB(f2.toppoint, 10, 110);

  for (int i = 117; i > 5; i--)
  {
    if (
        (IMG_120_160[i][80] != 0) &&
        (IMG_120_160[i - 1][80] == 0))
    {
      f2.toprow_80col = i;
      break;
    }
  }
  __Limit_AB(f2.toprow_80col, 10, 110);
}

void find_L_egde_from_R()
{
  for (int i = 118; i > 0; i--)
  {
    f1.tag_leftline[i] = 0;
    f1.tag_leftlineflag[i] = 0;
    for (int j = f1.tag_rightline[i + 1]; j > 0; j--)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] == 0)
      {
        f1.tag_leftline[i] = j - 1;
        f1.tag_leftlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_R_egde_from_L()
{
  for (int i = 118; i > 0; i--)
  {
    f1.tag_rightline[i] = 159;
    f1.tag_rightlineflag[i] = 0;
    for (int j = f1.tag_leftline[i + 1]; j < 159; j++)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] == 0)
      {
        f1.tag_rightline[i] = j + 1;
        f1.tag_rightlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_L_edge_from_outter()
{
  f1.tag_leftline[118] = f1.leftline[118];
  f1.tag_leftlineflag[118] = f1.leftlineflag[118];
  for (int i = 117; i > 0; i--)
  {
    int k1 = f1.tag_leftline[i + 1] + 6;
    int k2 = f1.tag_leftline[i + 1] - 6;
    __Limit_AB(k1, 0, 159);
    __Limit_AB(k2, 0, 159);
    f1.tag_leftline[i] = 0;
    f1.tag_leftlineflag[i] = 0;
    for (int j = k2; j <= k1; j++)
    {
      if (IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_leftline[i] = j - 1;
        f1.tag_leftlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_R_edge_from_outter()
{
  f1.tag_rightline[118] = f1.rightline[118];
  f1.tag_rightlineflag[118] = f1.rightlineflag[118];
  for (int i = 117; i > 0; i--)
  {
    int k3 = f1.tag_rightline[i + 1] + 6;
    int k4 = f1.tag_rightline[i + 1] - 6;
    __Limit_AB(k3, 0, 159);
    __Limit_AB(k4, 0, 159);
    f1.tag_rightline[i] = 159;
    f1.tag_rightlineflag[i] = 0;
    for (int j = k3; j >= k4; j--)
    {
      if (IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_rightline[i] = j + 1;
        f1.tag_rightlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_L_edge_from_inner()
{
  f1.tag_leftline[118] = f1.leftline[118];
  f1.tag_leftlineflag[118] = f1.leftlineflag[118];
  for (int i = 117; i > 0; i--)
  {
    int k1 = f1.tag_leftline[i + 1] + 6;
    int k2 = f1.tag_leftline[i + 1] - 6;
    __Limit_AB(k1, 0, 159);
    __Limit_AB(k2, 0, 159);
    f1.tag_leftline[i] = 0;
    f1.tag_leftlineflag[i] = 0;
    for (int j = k1; j >= k2; j--)
    {
      if (IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_leftline[i] = j - 1;
        f1.tag_leftlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_R_edge_from_inner()
{
  f1.tag_rightline[118] = f1.rightline[118];
  f1.tag_rightlineflag[118] = f1.rightlineflag[118];
  for (int i = 117; i > 0; i--)
  {
    int k3 = f1.tag_rightline[i + 1] + 6;
    int k4 = f1.tag_rightline[i + 1] - 6;
    __Limit_AB(k3, 0, 159);
    __Limit_AB(k4, 0, 159);
    f1.tag_rightline[i] = 159;
    f1.tag_rightlineflag[i] = 0;
    for (int j = k4; j <= k3; j++)
    {
      if (IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_rightline[i] = j + 1;
        f1.tag_rightlineflag[i] = 1;
        break;
      }
    }
  }
}

void find_cross_infec()
{
  f2.L_infec_flag = 0;
  f2.L_infec_row = 118;
  f2.L_infec_col = 0;

  f2.R_infec_flag = 0;
  f2.R_infec_row = 118;
  f2.R_infec_col = 159;

  f2.L_up_infec_flag = 0;
  f2.L_up_infec_row = 0;
  f2.L_up_infec_col = 0;

  f2.R_up_infec_flag = 0;
  f2.R_up_infec_row = 0;
  f2.R_up_infec_col = 0;

  for (int i = 110; i > 45; i--)
  {
    if ((f1.leftline[i] < 140 &&
         f1.leftline[i] > 20 &&

         f1.leftlineflag[i + 3] == 1 &&
         f1.leftlineflag[i + 5] == 1 &&
         f1.leftlineflag[i] == 1 &&

         abs(f1.leftline[i] - f1.leftline[i + 3]) <= 6 &&

         (f1.leftlineflag[i - 5] == 0 ||
          f1.leftlineflag[i - 3] == 0 ||
          f1.leftlineflag[i - 10] == 0)) ||

        (f1.leftline[i] < 140 &&

         f1.leftline[i-2] > f1.leftline[i - 4] &&
         f1.leftline[i] > f1.leftline[i - 2] &&
         f1.leftline[i] > f1.leftline[i + 3] &&
         f1.leftline[i+3] > f1.leftline[i + 6] &&

        //  (f1.leftlineflag[i - 5] == 0 ||
        //   f1.leftlineflag[i - 3] == 0 ||
        //   f1.leftlineflag[i - 10] == 0) &&

         f1.leftlineflag[i + 3] == 1 &&
         f1.leftlineflag[i] == 1 &&
         2 * f1.leftline[i] - f1.leftline[i - 3] - f1.leftline[i + 3] > 4))
    {
      f2.L_infec_flag = 1;
      f2.L_infec_row = i;
      f2.L_infec_col = f1.leftline[i];
      break;
    }
  }

  for (int i = 110; i > 45; i--)
  {
    if ((f1.rightline[i] > 20 &&
         f1.rightline[i] < 140 &&

         f1.rightlineflag[i + 3] == 1 &&
         f1.rightlineflag[i + 5] == 1 &&
         f1.rightlineflag[i] == 1 &&

         abs(f1.leftline[i] - f1.leftline[i + 3]) <= 6 &&

         (f1.rightlineflag[i - 5] == 0 ||
          f1.rightlineflag[i - 3] == 0 ||
          f1.rightlineflag[i - 10] == 0)) ||

        (f1.rightline[i] > 20 &&

         f1.rightline[i] < f1.rightline[i - 3] &&
         f1.rightline[i] < f1.rightline[i + 6] &&
         f1.rightline[i] < f1.rightline[i - 5] &&

        //  (f1.rightlineflag[i - 5] == 0 ||
        //   f1.rightlineflag[i - 3] == 0 ||
        //   f1.rightlineflag[i - 10] == 0) &&
        f1.rightline[i-2] < f1.rightline[i - 4] &&
         f1.rightline[i] < f1.rightline[i - 2] &&
         f1.rightline[i] < f1.rightline[i + 3] &&
         f1.rightline[i+3] < f1.rightline[i + 6] &&


         f1.rightlineflag[i + 3] == 1 &&
         f1.rightlineflag[i] != 0 &&
         f1.rightline[i + 3] + f1.rightline[i - 3] - 2 * f1.rightline[i] > 4))
    {
      f2.R_infec_flag = 1;
      f2.R_infec_row = i;
      f2.R_infec_col = f1.rightline[i];
      break;
    }
  }

  if (is_L_R_lose(118, 100, 2, 100))
  {
    f2.L_infec_flag = 0;
    f2.R_infec_flag = 0;
  }

  if (f2.R_infec_flag && f2.L_infec_flag)
  {
    if (f2.R_infec_row - f2.L_infec_row > 30)
    {
      f2.L_infec_flag = 0;
      f2.L_infec_row = 118;
      f2.L_infec_col = 0;
    }
    else if (f2.L_infec_row - f2.R_infec_row > 30)
    {
      f2.R_infec_flag = 0;
      f2.R_infec_row = 118;
      f2.R_infec_col = 159;
    }

    if (f2.R_infec_row > f2.L_infec_row &&
        f2.R_infec_col < f2.L_infec_col)
    {
      f2.L_infec_flag = 0;
      f2.L_infec_row = 118;
      f2.L_infec_col = 0;
    }
    else if (f2.L_infec_row > f2.R_infec_row &&
             f2.R_infec_col < f2.L_infec_col)
    {
      f2.R_infec_flag = 0;
      f2.R_infec_row = 118;
      f2.R_infec_col = 159;
    }
  }

  f2.cross_start_row = MIN(f2.R_infec_row, f2.L_infec_row) - 10;
  if (f2.cross_start_row)
  {
    memset(f1.cross_down_line, 0, sizeof(f1.cross_down_line));
    memset(f1.cross_down_line_flag, 0, sizeof(f1.cross_down_line_flag));
    for (int i = 0; i < 160; i++)
    {
      for (int j = f2.cross_start_row; j > 20; j--)
      {
        if (
            IMG_120_160[j + 1][i] == 0 &&
            IMG_120_160[j][i] != 0)
        {
          break;
        }
        if (
            IMG_120_160[j + 1][i] != 0 &&
            IMG_120_160[j][i] == 0)
        {
          f1.cross_down_line[i] = j - 1;
          f1.cross_down_line_flag[i] = 1;
          break;
        }
      }
    }

    for (int i = MAX(f2.L_infec_col - 20, 0); i < f2.R_infec_col - 8; i++)
    {
      if (
          (abs(f1.cross_down_line[i] - f1.cross_down_line[i + 3]) -
           abs(f1.cross_down_line[i] - f1.cross_down_line[i - 3])) >= 3 &&

          f1.cross_down_line_flag[i] &&
          f1.cross_down_line_flag[i - 3] &&
          f1.cross_down_line_flag[i + 3] &&
          f1.cross_down_line_flag[i - 6] &&

          (f1.leftlineflag[f1.cross_down_line[i] + 5] == 0 ||
           f1.leftlineflag[f1.cross_down_line[i] + 10] == 0 ||
           f1.leftlineflag[f1.cross_down_line[i] + 15] == 0) &&

          f1.cross_down_line[i] > 30)
      {
        f2.L_up_infec_flag = 1;
        f2.L_up_infec_row = f1.cross_down_line[i];
        f2.L_up_infec_col = i;
        break;
      }
    }

    if (!f2.L_up_infec_flag)
    {
      for (int i = f2.cross_start_row; i > f2.toppoint; i--)
      {
        if (
            f1.leftlineflag[i - 3] == 1 &&
            f1.leftlineflag[i - 5] == 1 &&
            f1.leftlineflag[i] == 1 &&

            abs(f1.leftline[i] - f1.leftline[i - 3]) <= 5 &&

            (f1.leftlineflag[i + 5] == 0 ||
             f1.leftlineflag[i + 10] == 0 ||
             f1.leftlineflag[i + 15] == 0))
        {
          f2.L_up_infec_flag = 1;
          f2.L_up_infec_row = i;
          f2.L_up_infec_col = f1.leftline[i];
          break;
        }
      }
    }

    for (int i = MIN(f2.R_infec_col + 20, 159); i > f2.L_infec_col + 8; i--)
    {
      if (
          (abs(f1.cross_down_line[i] - f1.cross_down_line[i - 3]) -
           abs(f1.cross_down_line[i] - f1.cross_down_line[i + 3])) >= 3 &&

          f1.cross_down_line_flag[i] &&
          f1.cross_down_line_flag[i + 3] &&
          f1.cross_down_line_flag[i - 3] &&
          f1.cross_down_line_flag[i + 6] &&

          (f1.rightlineflag[f1.cross_down_line[i] + 5] == 0 ||
           f1.rightlineflag[f1.cross_down_line[i] + 10] == 0 ||
           f1.rightlineflag[f1.cross_down_line[i] + 15] == 0) &&

          f1.cross_down_line[i] > 30)
      {
        f2.R_up_infec_flag = 1;
        f2.R_up_infec_row = f1.cross_down_line[i];
        f2.R_up_infec_col = i;
        break;
      }
    }

    if (!f2.R_up_infec_flag)
    {
      for (int i = f2.toppoint; i < 90; i++)
      {
        if (
            f1.rightlineflag[i - 3] == 1 &&
            f1.rightlineflag[i - 5] == 1 &&
            f1.rightlineflag[i] == 1 &&

            abs(f1.rightline[i] - f1.rightline[i - 3]) <= 5 &&

            (f1.rightlineflag[i + 5] == 0 ||
             f1.rightlineflag[i + 10] == 0 ||
             f1.rightlineflag[i + 15] == 0))
        {
          f2.R_up_infec_flag = 1;
          f2.R_up_infec_row = i;
          f2.R_up_infec_col = f1.rightline[i];
          break;
        }
      }
    }
  }

  if (f2.R_up_infec_row > 105)
  {
    f2.R_up_infec_flag = 0;
  }

  if (f2.L_up_infec_row > 105)
  {
    f2.L_up_infec_flag = 0;
  }
}

void find_topstart_row()
{
  f1.tag_L_start_row = 118;
  f1.tag_R_start_row = 118;
  f2.tag_top_row = 10;

 for (int i = 105; i > 10; i--)
 {
   if (abs(f1.tag_leftline[i] - f1.tag_leftline[i - 1]) > 8 ||
       abs(f1.tag_rightline[i] - f1.tag_rightline[i - 1]) > 8 ||
       (f1.tag_rightline[i] - f1.tag_leftline[i]) - 2 * half_width[i] > 16 ||
       (f1.tag_rightline[i] - f1.tag_leftline[i]) - (f1.tag_rightline[i + 1] - f1.tag_leftline[i + 1]) > 8)
   {
     f2.tag_top_row = i;
     break;
   }
 }

  for (int i = 115; i > f2.tag_top_row; i--)
  {
    if (
        (f1.tag_leftlineflag[i] &&
         !f1.tag_leftlineflag[i + 1]) ||
        abs(f1.tag_leftline[i] - f1.tag_leftline[i + 1]) > 6)
    {
      f1.tag_L_start_row = i - 1;
      break;
    }
  }

  for (int i = 115; i > f2.tag_top_row; i--)
  {
    if (
        (f1.tag_rightlineflag[i] &&
         !f1.tag_rightlineflag[i + 1]) ||
        abs(f1.tag_rightline[i] - f1.tag_rightline[i + 1]) > 8)
    {
      f1.tag_R_start_row = i - 1;
      break;
    }
  }
}

void find_L_tag_toppoint()
{
  f1.tag_toppoint = 0;
  for (int i = 115; i > 5; i--)
  {
    int mid_tmp = 80;
    mid_tmp = f1.tag_leftline[i] + half_width[i];

    if (
        ((IMG_120_160[i][mid_tmp] != 0) &&
         (IMG_120_160[i - 1][mid_tmp] == 0 ||
          IMG_120_160[i - 2][mid_tmp] == 0)) ||
        (mid_tmp >= 159 || mid_tmp <= 0))
    {
      f1.tag_toppoint = i;
      break;
    }
  }
}

void find_R_tag_toppoint()
{
  f1.tag_toppoint = 0;
  for (int i = 115; i > 5; i--)
  {
    int mid_tmp = 80;
    mid_tmp = f1.tag_rightline[i] - half_width[i];

    if (
        ((IMG_120_160[i][mid_tmp] != 0) &&
         (IMG_120_160[i - 1][mid_tmp] == 0 ||
          IMG_120_160[i - 2][mid_tmp] == 0)) ||
        (mid_tmp >= 159 || mid_tmp <= 0))
    {
      f1.tag_toppoint = i;
      break;
    }
  }
}

void find_road_duty()
{
  for (int i = 118; i > 0; i--)
  {
    f1.road_real_width[i] = f1.tag_rightline[i] - f1.tag_leftline[i];
    int black_point_num = 0;
    f1.tag_black_duty[i] = 0;
    for (int j = f1.tag_leftline[i]; j < f1.tag_rightline[i]; j++)
    {
      if (IMG_120_160[i][j] == 0)
        black_point_num++;
    }

    if (f1.road_real_width[i] != 0)
      f1.tag_black_duty[i] = black_point_num * 100 / f1.road_real_width[i];
  }
}

void find_garage_row()
{
  f2.garage_row = 0;
  f2.garage_tmp[0] = 0;
  f2.garage_tmp[1] = 0;
  f2.garage_start_row = 0;

  for (int16 i = 115; i >= 45; i--)
  {
    for (int16 j = 30; j < 130; j++)
    {
      if (IMG_120_160[i][j - 1] != 0 && // != 0 ��ɫ
          IMG_120_160[i][j - 2] != 0 &&
          IMG_120_160[i][j] == 0) // == 0 ��ɫ
      {
        f2.garage_tmp[0] = j;
      }

      if (IMG_120_160[i][j - 1] == 0 &&
          IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0)
      {
        if (f2.garage_tmp[0] != 0)
          f2.garage_tmp[1] = j;
      }

      if (f2.garage_tmp[0] != 0 &&
          f2.garage_tmp[1] != 0 &&
          f2.garage_tmp[1] - f2.garage_tmp[0] >= 2)
      {
        f2.garage_tmp[0] = 0;
        f2.garage_tmp[1] = 0;
        f2.garage_block_cnt++;
      }

      if (f2.garage_block_cnt >= 4)
      {

        if (!f2.garage_start_row)
          f2.garage_start_row = i;

        f2.garage_row++;
        f2.garage_block_cnt = 0;
        break;
      }
    }
    f2.garage_block_cnt = 0;
  }
}

void long_judge()
{
  f2.long_straight_flag = 0;

  if (
      f2.toppoint <= LONG_STRAIGHT_ENDROW &&
      is_M_center(118, LONG_STRAIGHT_ENDROW, 8) &&
      is_L_R_nonlose(LONG_STRAIGHT_ENDROW, 100, 2, 92))
  {
    f2.long_straight_flag = 1;
  }
}

void short_judge()
{
  int16 stdDeviation = (int16)solve_variance(100, 40);
  if (stdDeviation <= 6)
  {
    f2.shortstraight_flag = 1;
  }
  else if (stdDeviation <= 10)
  {
    f2.shortstraight_flag = 2;
  }
  else
  {
    f2.shortstraight_flag = 0;
  }
}

void stop_judge()
{
  f2.stop_car_flag = 0;
  f2.stop_car_reg[0] = 0;

  if (is_region_one_color(1, 113, 158, 118, 0, 90))
    f2.stop_car_reg[0] = 1;

  int stop_count = 0;
  for (int i = 19; i >= 1; i--)
  {
    f2.stop_car_reg[i] = f2.stop_car_reg[i - 1];
    if (f2.stop_car_reg[i])
      stop_count++;
  }

  if (stop_count >= 8)
    f2.stop_car_flag = 1;
}

void L_mid_endrow_proc()
{
  if (f2.L_mid_endrow == 0)
    f2.L_mid_endrow = f2.toppoint;
  for (int i = f2.L_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 0;
    f1.rightline[i] = 0;
    f1.midline[i] = 0;
  }
}

void R_mid_endrow_proc()
{
  if (f2.R_mid_endrow == 0)
    f2.R_mid_endrow = f2.toppoint;
  for (int i = f2.R_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 159;
    f1.rightline[i] = 159;
    f1.midline[i] = 159;
  }
}

void find_mem_L_edge()
{
  int k1;
  int k2;
  for (int i = 118; i > 0; i--)
  {
    if (i == 118)
    {
      k1 = f1.tag_leftline[i] + 6;
      k2 = f1.tag_leftline[i] - 6;
    }
    else
    {
      k1 = f1.tag_leftline[i + 1] + 6;
      k2 = f1.tag_leftline[i + 1] - 6;
    }
    __Limit_AB(k1, 0, 159);
    __Limit_AB(k2, 0, 159);
    f1.tag_leftline[i] = 0;
    for (int j = k1; j >= k2; j--)
    {
      if (IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_leftline[i] = j - 1;
        break;
      }
    }
  }
}

void find_mem_R_edge()
{
  int k3;
  int k4;

  for (int i = 118; i > 0; i--)
  {
    if (i == 118)
    {
      k3 = f1.tag_rightline[i] + 6;
      k4 = f1.tag_rightline[i] - 6;
    }
    else
    {
      k3 = f1.tag_rightline[i + 1] + 6;
      k4 = f1.tag_rightline[i + 1] - 6;
    }
    __Limit_AB(k3, 0, 159);
    __Limit_AB(k4, 0, 159);
    f1.tag_rightline[i] = 159;
    for (int j = k4; j <= k3; j++)
    {
      if (IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f1.tag_rightline[i] = j + 1;
        break;
      }
    }
  }
}

void midline_proc2(int i)
{
  if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
  {
    f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
  }
  else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1)
  {
    f1.leftline[i] = 0;
    f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
    if (i < 99 && f1.rightline[i] <= f1.rightline[i + 1] &&
        ((f1.leftlineflag[i + 8] == 0 && f1.rightlineflag[i + 8] == 0) ||
         (f1.leftlineflag[i + 11] == 0 && f1.rightlineflag[i + 11] == 0) ||
         (f1.leftlineflag[i + 15] == 0 && f1.rightlineflag[i + 15] == 0))

    )
    {
      f1.midline[i] = f1.midline[i + 1];
    }
  }
  else if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 0)
  {
    f1.rightline[i] = 159;
    f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
    if (i < 99 && f1.leftline[i] >= f1.leftline[i + 1] &&

        ((f1.leftlineflag[i + 8] == 0 && f1.rightlineflag[i + 8] == 0) ||
         (f1.leftlineflag[i + 11] == 0 && f1.rightlineflag[i + 11] == 0) ||
         (f1.leftlineflag[i + 15] == 0 && f1.rightlineflag[i + 15] == 0)))
    {
      f1.midline[i] = f1.midline[i + 1];
    }
  }
  else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0)
  {
    f1.leftline[i] = 0;
    f1.rightline[i] = 159;
    if (i == 118)
      f1.midline[i] = 80;
    else
      f1.midline[i] = f1.midline[i + 1];
  }

  f1.real_width[i] = f1.rightline[i] - f1.leftline[i];
  __Limit_AB(f1.midline[i], 0, 159);
  f2.column_start = f1.midline[i];

  if (f2.fork_sm_state != 0 && (f2.fork1_dir == 0 || f2.fork2_dir == 0))
    f2.column_start = f1.midline[i] - f1.real_width[i] / 3;
  if (f2.fork_sm_state != 0 && (f2.fork1_dir == 1 || f2.fork2_dir == 0))
    f2.column_start = f1.midline[i] + f1.real_width[i] / 3;

  __Limit_AB(f2.column_start, 0, 159);
}

void find_track_gray()
{
  if (f1.reset_ele_signal)
  {
    f2.column_start = 80;
  }

  int16 i = 0, j = 0, k = 0;
  f1.leftstartrow = 0;
  f1.rightstartrow = 0;

  for (i = 118; i > 0; i--)
  {
    k = f2.column_start;
    f1.leftline[i] = 0;
    f1.leftlineflag[i] = 0;
    f1.midline[i] = 80;
    f1.rightline[i] = 159;
    f1.rightlineflag[i] = 0;

    for (j = k; j > 2; j--)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] == 0)
      {
        if (f1.leftstartrow == 0)
          f1.leftstartrow = i;
        f1.leftline[i] = j - 1;
        f1.leftlineflag[i] = 1;
        break;
      }
    }
    for (j = k; j < 157; j++)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j + 2] == 0)
      {
        if (f1.rightstartrow == 0)
          f1.rightstartrow = i;
        f1.rightline[i] = j + 1;
        f1.rightlineflag[i] = 1;
        break;
      }
    }

    midline_proc2(i);
  }

  f2.column_start = f1.midline[118];
  find_toppoint();
  f2.toppoint_reg = f2.toppoint;
}

void set_fork_flag()
{
  f2.fork_num++;
  f2.fork_sm_state = 1;
  chassis.fork_distance = 0;
  chassis.all_distance = 0;

  inoutdev.beep(1);
  disable_all_elements();
  f1.fork_enable = 1;

  find_track_gray();
}

void set_L_loop_flag()
{
  inoutdev.beep(2);
  f2.L_loop_sm_state = 1;
  disable_all_elements();
  f1.loop_enable = 1;

  f2.loop_num++;
  inoutdev.gyro_z_angle = 0;
  chassis.all_distance = 0;
  chassis.loop_distance = 0;
  f1.L_loop_start_dis = chassis.row_to_distance(f2.L_loop_circle_row);
}

void set_R_loop_flag()
{
  inoutdev.beep(2);
  f2.R_loop_sm_state = 1;
  disable_all_elements();
  f1.loop_enable = 1;

  f2.loop_num++;
  inoutdev.gyro_z_angle = 0;
  chassis.all_distance = 0;
  chassis.loop_distance = 0;
  f1.R_loop_start_dis = chassis.row_to_distance(f2.R_loop_circle_row);
}

void set_garage_flag()
{
  inoutdev.beep(1);
  disable_all_elements();
  f1.garage_enable = 1;

  chassis.all_distance = 0;
  chassis.start_distance = 0;
  inoutdev.start_angle = 0;
  f1.current_turns++;
  f2.start_num++;
  inoutdev.gyro_y_angle = 0;
  f1.garage_regco_dis = chassis.row_to_distance(f2.garage_start_row);

  find_L_edge_from_outter();
  find_R_edge_from_outter();

  if (f1.current_turns == 1)
  {
    f2.garage_sm_state = 5;
    // f2.fork_dir = !f2.fork_dir;

    f2.fork1_dir = !f2.fork1_dir;
    f2.fork2_dir = !f2.fork2_dir;
  }
  else
  {
    f2.garage_sm_state = 4;
  }
}

void set_qrcode_flag()
{
  find_L_edge_from_outter();
  find_R_edge_from_outter();

  f2.qrcode_num++;
  chassis.tag_distance = 0;
  chassis.all_distance = 0;

  inoutdev.lbeep(1);
  f2.tag_sm_state = 1;
  disable_all_elements();
  f1.tag_enable = 1;
}

void set_ramp_flag()
{
  inoutdev.lbeep(1);
  f2.ramp_sm_state = 1;
  chassis.ramp_distance = 0;
  inoutdev.gyro_y_angle = 0;
  f2.ramp_num ++;

  disable_all_elements();
  f1.ramp_enable = 1;
}

void s_cross_judge()
{
  f2.s_cross_flag = 0;
  f2.cross_type = 0;

  for (int i = 100; i >= 30; i--)
  {
    if ( //i==59,18+6=24
        f1.leftline[i] > 60 && f1.leftline[i] <= 150 &&
        (f1.leftline[i] - f1.leftline[i + 6]) + (f1.leftline[i] - f1.leftline[i - 6]) >= 12 &&
        f1.leftline[i] > f1.leftline[i + 8] + 5 &&
        f1.leftline[i] > f1.leftline[i - 8] + 8 &&
        f1.leftlineflag[i] &&
        f1.leftlineflag[i - 6] &&
        f1.leftlineflag[i + 6] &&
        is_straight_line(i, i + 5, f1.leftline, 4) &&
        is_straight_line(i, i - 5, f1.leftline, 5) &&
        f1.rightlineflag[i-3] == 0 &&
        f1.rightlineflag[i-5] == 0 &&
        f1.rightlineflag[i-7] == 0)
    {
      f2.L_infec_row = i;
      f2.L_infec_col = f1.leftline[i];
      f2.L_infec_flag = 1;

      f2.s_cross_flag = 1;
      f2.cross_type = 4;
      break;
    }
    else if (
        f1.rightline[i] < 100 && f1.rightline[i] >= 10 &&
        ((f1.rightline[i - 6] - f1.rightline[i]) + (f1.rightline[i + 6] - f1.rightline[i])) >= 12 &&
        f1.rightline[i] < f1.rightline[i + 8] - 5 &&
        f1.rightline[i] < f1.rightline[i - 8] - 8 &&
        f1.rightlineflag[i] &&
        f1.rightlineflag[i - 6] &&
        f1.rightlineflag[i + 6] &&
        is_straight_line(i, i + 5, f1.rightline, 4) &&
        is_straight_line(i, i - 5, f1.rightline, 5) &&
        f1.leftlineflag[i-3] == 0 &&
        f1.leftlineflag[i-5] == 0 &&
        f1.leftlineflag[i-7] == 0)
    {
      f2.R_infec_row = i;
      f2.R_infec_flag = 1;
      f2.R_infec_col = f1.rightline[i];

      f2.s_cross_flag = 1;
      f2.cross_type = 5;
      break;
    }
  }
}

void find_elements()
{
  if (f1.reset_ele_signal)
  {
    f2 = {0};
    f1.reset_ele_signal = 0;
    enable_all_elements();
  }

  f2.L_mid_endrow = 0;
  f2.R_mid_endrow = 0;
//300 fork-->loop
//1145 loop-->fork

if (f2.garage_dir == 1)
  {
    if (f2.L_loop_sm_state!=0||
      (f2.R_loop_sm_state != 0&&f2.ramp_sm_state==0) ||
        (f2.loop_num == 0&&f2.ramp_sm_state==0) ||
        (f2.ramp_num%2 == 0 && chassis.all_distance >= 40&&f2.ramp_sm_state==0))
    {
      f1.loop_enable = 1;
    }
    else
    {
      f1.loop_enable = 0;
    }


    if ((f2.fork_sm_state != 0&&f2.ramp_sm_state==0) ||
        (f2.ramp_num%2 == 1 && chassis.all_distance >= 50&&f2.ramp_sm_state==0))
      {
        f1.fork_enable = 1;
      }
      else
      {
        f1.fork_enable = 0;
      }
  }


  if (f1.ele_enable)
  {
#ifdef ENABLE_RAMP
    if (f1.ramp_enable)
    {
      if (f2.ramp_sm_state == ramp_judge_sm)
      {
        // ramp_judge();
        // if(f2.ramp_flag)
        // {
        //       inoutdev.lbeep(1);
        //     f2.ramp_sm_state = 1;
        //     chassis.ramp_distance = 0;
        //     inoutdev.gyro_y_angle = 0;
        //     chassis.all_distance=0;
        //     disable_all_elements();
        //     f1.ramp_enable = 1;
        // }
        
        // if (
        // (chassis.all_distance>870 && f2.loop_num == (f2.fork_num/2) + 1) ||
        // (f2.loop_num == (f2.fork_num/2) && chassis.all_distance > 205))

        if ((f2.loop_num == ((f2.fork_num/2) + 1) && chassis.all_distance >= 550) ||
            (f2.loop_num == (f2.fork_num/2) && chassis.all_distance >= 205 && f2.ramp_num%2 == 1))
        {
          // set_ramp_flag();
            inoutdev.lbeep(1);
            f2.ramp_sm_state = 1;
            chassis.ramp_distance = 0;
            inoutdev.gyro_y_angle = 0;
            chassis.all_distance=0;
            disable_all_elements();
            f1.ramp_enable = 1;
        }
      }

      if (f2.ramp_sm_state == 1)
      {
        if(chassis.all_distance>=140)
        {
          inoutdev.lbeep(1);
          f2.ramp_sm_state = ramp_judge_sm;
          chassis.ramp_distance = 0;
          chassis.all_distance=0;
          enable_all_elements();
            f2.ramp_num ++;
        }
      }

      // if (f2.ramp_sm_state == ramp_before)
      // {
      //   if (
      //       fabs(inoutdev.gyro_y_angle) > 8 )
      //   {
      //     f2.ramp_sm_state = ramp_uping;
      //     chassis.ramp_distance = 0;
      //   }
      // }

      // if (f2.ramp_sm_state == ramp_uping)
      // {
      //   if (
      //       fabs(inoutdev.gyro_y_angle) <= 4 )
      //   {
      //     f2.ramp_sm_state = ramp_on_top;
      //     chassis.ramp_distance = 0;
      //   }
      // }

      // if (f2.ramp_sm_state == ramp_on_top)
      // {
      //   if (fabs(inoutdev.gyro_y_angle) > 8)
      //   {
      //     f2.ramp_sm_state = ramp_downing;
      //     chassis.ramp_distance = 0;
      //   }
      // }

      // if (f2.ramp_sm_state == ramp_downing)
      // {
      //   if (fabs(inoutdev.gyro_y_angle) <= 4 )
      //   {
      //     inoutdev.lbeep(1);
      //     f2.ramp_sm_state = ramp_judge_sm;
      //     chassis.ramp_distance = 0;
      //     chassis.all_distance=0;
      //     enable_all_elements();
      //     f2.ramp_num += 1;
      //   }
      // }
    }
#endif


// #ifdef ENABLE_RAMP
//     if (f1.ramp_enable)
//     {
//       if (f2.ramp_sm_state == ramp_judge_sm)
//       {
//         if (fabs(inoutdev.gyro_y_angle) >= 9) //16 max
//             {
//               f2.ramp_flag = 1;
//               f2.ramp_frames++;
//             }
//             else
//             {
//               f2.ramp_flag = 0;
//               f2.ramp_frames = 0;
//             }

//         if (f2.ramp_flag)
//         {
//           // set_ramp_flag();
//           inoutdev.lbeep(1);
//           f2.ramp_sm_state = 1;
//           chassis.ramp_distance = 0;
//           disable_all_elements();
//           f1.ramp_enable = 1;
//         }
//       }

//       if (f2.ramp_sm_state == ramp_before)
//       {
//           f2.ramp_sm_state = ramp_uping;
//           chassis.ramp_distance = 0;
//       }

//       if (f2.ramp_sm_state == ramp_uping)
//       {
//         if (
//             fabs(inoutdev.gyro_y_angle) <= 4)
//         {
//           f2.ramp_sm_state = ramp_on_top;
//           chassis.ramp_distance = 0;
//         }
//       }

//       if (f2.ramp_sm_state == ramp_on_top)
//       {
//         if (inoutdev.gyro_y_angle <= -8)
//         {
//           f2.ramp_sm_state = ramp_downing;
//           chassis.ramp_distance = 0;
//         }
//       }

//       if (f2.ramp_sm_state == ramp_downing)
//       {
//         if (fabs(inoutdev.gyro_y_angle) <= 4)
//         {
//           inoutdev.lbeep(1);
//           f2.ramp_sm_state = ramp_judge_sm;
//           chassis.ramp_distance = 0;
//           enable_all_elements();
//         }
//       }
//     }
// #endif

#ifdef ENABLE_START
    if (f1.start_enable && f2.en_start)
    {
      if (f2.start_sm_state == start_judge_sm)
      {
        inoutdev.beep(1);

        chassis.start_distance = 0;
        chassis.all_distance = 0;
        inoutdev.start_angle = 0;
        f2.start_num++;

        f2.start_sm_state = start_outing;
        disable_all_elements();
        f1.start_enable = 1;
      }

      if (f2.start_sm_state == start_end)
      {
        chassis.start_distance = 0;
        f2.start_sm_state = 0;
        f2.en_start = 0;

        inoutdev.start_angle = 0;
        enable_all_elements();
        inoutdev.gyro_y_angle = 0;
      }

      if (f2.start_sm_state == start_outing)
      {
        if (chassis.start_distance > 3)
        {
          f2.start_sm_state = start_angleing;
          chassis.start_distance = 0;
        }
        for (int i = 118; i > 10; i--)
        {
          f1.midline[i] = 80;
        }
        f2.toppoint = 10;
        return;
      }

      if (f2.start_sm_state == start_angleing)
      {
        if (fabs(inoutdev.start_angle) > 82 ||
            chassis.start_distance > 60)
        {
          f2.start_sm_state = start_end;
        }
        for (int i = 118; i > 10; i--)
        {
          if (f2.garage_dir == 0)
            f1.midline[i] = 0;
          else if (f2.garage_dir == 1)
            f1.midline[i] = 159;
        }
        f2.toppoint = 10;
        return;
      }
    }
#endif

// #ifdef ENABLE_FORK
//     if (f1.fork_enable)
//     {
//       if (f2.fork_sm_state == fork_judge_sm)
//       {
//         fork_road_judge();
//         if (f2.fork_flag)
//         // if (f2.fork_flag||(f2.fork_num%2==1))
//         {
//           set_fork_flag();
//         }
//       }

//       if (f2.fork_sm_state == fork_before ||
//           f2.fork_sm_state == fork_in)
//       {
//         if (f2.fork1_dir == 1)
//         {
//           R_fork_patch();
//           fork_road_judge();

//           if (f2.fork_sm_state == fork_before)
//             if (f1.leftstartrow < 75 &&
//                 chassis.fork_distance > 10)
//             {
//               f2.fork_sm_state = fork_in;
//               chassis.fork_distance = 0;
//             }

//           if (f2.fork_sm_state == fork_in &&
//               !f2.fork_flag)
//             // if (f1.leftstartrow >110 &&
//             //     chassis.fork_distance > 10)
//             if (chassis.fork_distance > 240)
//             {
//               inoutdev.beep(1);
//               f2.fork_sm_state = 0;
//               enable_all_elements();
//               inoutdev.gyro_y_angle = 0;

//               //TODO
//             // if (f2.fork_num % 2 == 0)
//             // {
//                 f2.fork1_dir = !f2.fork1_dir;
//                 // f2.fork2_dir = !f2.fork2_dir;
//             // }
//             }
//         }

//         if (f2.fork1_dir == 0)
//         {
//           L_fork_patch();
//           fork_road_judge();

//           if (f2.fork_sm_state == fork_before)
//             if (f1.rightstartrow < 75 &&
//                 chassis.fork_distance > 10)
//             {
//               f2.fork_sm_state = fork_in;
//               chassis.fork_distance = 0;
//             }

//           if (f2.fork_sm_state == fork_in &&
//               !f2.fork_flag)
//             // if (f1.rightstartrow >110 &&
//             //     chassis.fork_distance > 10)
//             if (chassis.fork_distance > 240)
//             {
//               inoutdev.beep(1);
//               f2.fork_sm_state = 0;
//               enable_all_elements();
//               inoutdev.gyro_y_angle = 0;

//               //TODO
//             // if (f2.fork_num % 2 == 0)
//             // {
//                 f2.fork1_dir = !f2.fork1_dir;
//                 // f2.fork2_dir = !f2.fork2_dir;
//             // }
//             }
//         }
//       }
//     }
// #endif

#ifdef ENABLE_FORK
    if (f1.fork_enable)
    {
      if (f2.fork_sm_state == fork_judge_sm)
      {
        fork_road_judge();
        // if (f2.fork_flag)
        if (f2.fork_flag||(f2.fork_num%2==1&&chassis.fork_distance>155))
        {
          set_fork_flag();
        }
      }

      if (f2.fork_sm_state == fork_before ||
          f2.fork_sm_state == fork_in)
      {
        if (f2.fork1_dir == 1)
        {
          R_fork_patch();
          fork_road_judge();

          if (f2.fork_sm_state == fork_before)
            if (f1.leftstartrow < 75 &&
                chassis.fork_distance > 10)
            {
              f2.fork_sm_state = fork_in;
              chassis.fork_distance = 0;
            }

          if (f2.fork_sm_state == fork_in &&
              !f2.fork_flag)
            if (f1.leftstartrow >105 &&
                chassis.fork_distance > 10)
            // if (chassis.fork_distance > 240)
            {
              inoutdev.beep(1);
              f2.fork_sm_state = 0;
              enable_all_elements();
              inoutdev.gyro_y_angle = 0;

              if(f2.fork_num%2==0)
                f2.fork1_dir = !f2.fork1_dir;

              chassis.fork_distance=0;
            }
        }

        if (f2.fork1_dir == 0)
        {
          L_fork_patch();
          fork_road_judge();

          if (f2.fork_sm_state == fork_before)
            if (f1.rightstartrow < 75 &&
                chassis.fork_distance > 10)
            {
              f2.fork_sm_state = fork_in;
              chassis.fork_distance = 0;
            }

          if (f2.fork_sm_state == fork_in &&
              !f2.fork_flag)
            if (f1.rightstartrow >105 &&
                chassis.fork_distance > 10)
            // if (chassis.fork_distance > 240)
            {
              inoutdev.beep(1);
              f2.fork_sm_state = 0;
              enable_all_elements();
              inoutdev.gyro_y_angle = 0;

              if(f2.fork_num%2==0)
              f2.fork1_dir = !f2.fork1_dir;
              chassis.fork_distance=0;
            }
        }
      }
    }
#endif

#ifdef ENABLE_LOOP
    if (f1.loop_enable)
    {
      if (f2.L_loop_sm_state == 0 && f2.R_loop_sm_state == 0)
      {
        L_loop_judge();
        if (f2.L_loop_flag == 1&&
        f2.L_loop_frame>=1)
        {
          set_L_loop_flag();
        }
      }

      if (f2.L_loop_sm_state == 0 && f2.R_loop_sm_state == 0)
      {
        R_loop_judge();
        if (f2.R_loop_flag == 1&&
        f2.R_loop_frame>=1)
        {
          set_R_loop_flag();
        }
      }

      if (f2.L_loop_sm_state != 0)
      {
        L_loop_patch();
        find_post_proc();
        return;
      }

      if (f2.R_loop_sm_state != 0)
      {
        R_loop_patch();
        find_post_proc();
        return;
      }
    }
#endif
  }

  if (f2.ramp_sm_state == 0 &&
      f2.start_sm_state == 0 &&
      f2.tag_sm_state == 0 &&
      f2.garage_sm_state == 0 &&
      f2.fork_sm_state == 0 &&
      f2.L_loop_sm_state == 0 &&
      f2.R_loop_sm_state == 0)
  {
    cross_judge();
    if (f2.cross_flag == 1)
    {
      cross_patch();
      find_post_proc();
      return;
    }
  }

  if (
      f2.start_sm_state == 0 &&
      f2.tag_sm_state == 0 &&
      f2.garage_sm_state == 0 &&
      f2.fork_sm_state == 0 &&
      f2.L_loop_sm_state == 0 &&
      f2.R_loop_sm_state == 0)
  {
    find_little_curve();
    find_large_curve();

    if (f2.leftturnflag != 0 ||
        f2.rightturnflag != 0)
    {
      curve_patch();
      find_post_proc();
      return;
    }
  }

  if (
      f1.s_cross_enable &&
      f2.ramp_sm_state == 0 &&
      f2.start_sm_state == 0 &&
      //f2.tag_sm_state == 0 &&
      //f2.garage_sm_state == 0 &&
      f2.fork_sm_state == 0 &&
      f2.L_loop_sm_state == 0 &&
      f2.R_loop_sm_state == 0)
  {
    s_cross_judge();
    if (f2.s_cross_flag == 1)
    {
      //inoutdev.beep(1);
      f2.cross_K_L = 0;
      if (f2.cross_type == 4)
      {
        f2.cross_K_L = (float)((f1.leftline[f2.L_infec_row] - f1.leftline[f2.L_infec_row + 5]) / 5);
        for (int i = f2.L_infec_row; i > 20; i--)
        {
          f1.leftline[i] = (int)(f1.leftline[f2.L_infec_row] + (f2.cross_K_L) * (f2.L_infec_row - i));
          
          f1.rightline[i] = 159;
          f1.midline[i] = f1.leftline[i] + half_width[i];
          //__Limit_AB(f1.leftline[i], 0, 159);
          if (f1.midline[i] >= 159)
          {
            f1.midline[i] = 159;
            f2.R_mid_endrow = i;
            break;
          }
        }
        R_mid_endrow_proc();
      }
      if (f2.cross_type == 5)
      {
        f2.cross_K_R = 0;
        f2.cross_K_R = (float)((f1.rightline[f2.R_infec_row] - f1.rightline[f2.R_infec_row + 5]) / 5);
        for (int i = f2.R_infec_row; i > 20; i--)
        {
          f1.rightline[i] = (int)(f1.rightline[f2.R_infec_row] + (f2.cross_K_R) * (f2.R_infec_row - i));
          f1.leftline[i] = 0;
          f1.midline[i] = f1.rightline[i] - half_width[i];
          //__Limit_AB(f1.rightline[i], 0, 159);
          if (f1.midline[i] <= 0)
          {
            f1.midline[i] = 0;
            f2.L_mid_endrow = i;
            break;
          }
        }
        L_mid_endrow_proc();
        }
    find_post_proc();
    return;
    }
  }

  find_post_proc();
}

//判断输入的数组为err<scale的直线则返回1
//scale: 1~4
int is_straight_line(int start_index, int end_index, int16 line_[IMG_H], int scale)
{
  float k;
  float error;

  if (start_index == end_index)
  {
    return 1;
  }

  k = (float)(line_[start_index] - line_[end_index]) / (start_index - end_index); //斜率
  for (int i = (start_index < end_index ? start_index : end_index);
       i <= (start_index < end_index ? end_index : start_index); i++)
  {
    error = line_[i] - (k * (i - start_index) + line_[start_index]);
    if (fabs(error) >= scale)
      return 0;
  }

  return 1;
}

//判断输入的数组为连续的直线则返回1
//scale: 1~3
int is_continuous_line(int start, int end, int16 src_line[IMG_H], int threshold)
{
  if (start == end)
    return 0;

  if (start > end)
  {
    int tmp1 = end;
    end = start;
    start = tmp1;
  }

  //int cnt = 0;
  for (int i = start; i < end - 1; i++)
  {
    //threshold<变化<10,个数不多于3
    if (abs(src_line[i] - src_line[i + 1]) > threshold)
    {
      return 0;
      // if (abs(src_line[i] - src_line[i + 1]) > 10)
      // {
      //   return 0;
      // }
      // else
      // {
      //   cnt++;
      //   if (cnt >= 3)
      //     return 0;
      // }
    }
  }

  return 1;
}

//判断一个矩形区域内某颜色的占比大于threshold则返回1
//color 0:black;255:white
//(x1,y1) to (x2,y2)
//threshold: 0-100
int is_region_one_color(int x1, int y1, int x2, int y2, int color, int threshold)
{
  int total_cnt = 0;
  int white_cnt = 0;
  for (int i = x1; i <= x2; i++)
  {
    for (int j = y1; j <= y2; j++)
    {
      total_cnt++;
      if (IMG_120_160[j][i] == color)
        white_cnt++;
    }
  }
  int per = white_cnt * 100 / total_cnt;
  if (per > threshold)
    return 1;
  else
    return 0;
}

//分别判断是否丢线
//mode 0:left,1:right,2:both
//threshold: 0-100
//is_L_R_lose(40,120,2);
//i=40,i=120-1
int is_L_R_lose(int start, int end, int mode, int threshold)
{
  if (start == end)
    return 0;

  if (start > end)
  {
    int tmp1 = end;
    end = start;
    start = tmp1;
  }

  int lost_cnt = 0;
  int total_cnt = 0;

  for (int i = start; i < end; i++)
  {
    total_cnt++;

    if (mode == 0)
    {
      if (f1.leftlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
    else if (mode == 1)
    {
      if (f1.rightlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
    else
    {
      if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
  }
  if (lost_cnt * 100 / total_cnt >= threshold)
    return 1;
  else
    return 0;
}

//分别判断是否丢线
//mode 0:left,1:right,2:both
//threshold: 0-100
//is_L_R_nonlose(40,120,2);
//i=40,i=120-1
int is_L_R_nonlose(int start, int end, int mode, int threshold)
{
  if (start == end)
    return 0;

  if (start > end)
  {
    int tmp1 = end;
    end = start;
    start = tmp1;
  }

  int nonlost_cnt = 0;
  int total_cnt = 0;

  for (int i = start; i < end; i++)
  {
    total_cnt++;

    if (mode == 0)
    {
      if (f1.leftlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
    else if (mode == 1)
    {
      if (f1.rightlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
    else
    {
      if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
  }
  if (nonlost_cnt * 100 / total_cnt >= threshold)
    return 1;
  else
    return 0;
}

int is_M_center(int start, int end, int half_err)
{
  if (start == end)
    return 0;

  if (start > end)
  {
    int tmp1 = end;
    end = start;
    start = tmp1;
  }

  for (int i = start; i < end; i++)
  {
    if (f1.midline[i] > 80 + half_err ||
        f1.midline[i] < 80 - half_err)
    {
      return 0;
    }
  }
  return 1;
}

//计算一个矩形区域内某颜色的占比
//color 0:black;255:white
//(x1,y1) to (x2,y2)
//return: 0-100
int solve_region_duty(int x1, int y1, int x2, int y2, int color)
{
  int total_cnt = 0;
  int white_cnt = 0;
  for (int i = x1; i <= x2; i++)
  {
    for (int j = y1; j <= y2; j++)
    {
      total_cnt++;
      if (IMG_120_160[j][i] == color)
        white_cnt++;
    }
  }
  int per = white_cnt * 100 / total_cnt;
  return per;
}

float solve_variance(int begin_row, int end_row)
{
  if (begin_row == end_row)
    return 0;

  if (begin_row < end_row)
  {
    int tmp1 = begin_row;
    begin_row = end_row;
    end_row = tmp1;
  }

  int center_sum = 0;
  int count_center = 0;
  int center_ave = 0;
  int quadratic_sum = 0;

  for (int i = begin_row; i > end_row; i--)
  {
    center_sum += f1.midline[i];
    count_center++;
  }

  center_ave = (int)((float)center_sum / (float)count_center); //求得平均中值

  for (int i = begin_row; i > end_row; i--)
  {
    quadratic_sum += (f1.midline[i] - center_ave) * (f1.midline[i] - center_ave); //每行据平均中值得平方和
  }

  float variance = (float)quadratic_sum / (float)count_center;
  return (variance);
}

float solve_slope(int begin, int end, int16 *p)
{
  if (begin == end)
    return 0;

  if (begin > end)
  {
    int tmp1 = end;
    end = begin;
    begin = tmp1;
  }

  float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
  uint8 i = 0;
  float result = 0;
  static float resultlast;
  p = p + begin;
  for (i = begin; i < end; i++)
  {
    xsum += i;
    ysum += ((float)(*p));
    xysum += i * ((float)(*p));
    x2sum += i * i;
    p = p + 1;
  }
  if ((end - begin) * x2sum - xsum * xsum)
  {
    result = (float)((end - begin) * xysum - xsum * ysum) / (float)((end - begin) * x2sum - xsum * xsum);
    resultlast = result;
  }
  else
  {
    result = resultlast;
  }
  return result;
}
