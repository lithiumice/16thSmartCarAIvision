#include "f_elems.h"
#include "cstdlib"
#include "cstring"
Findline1_TypeDef f1;
Findline2_TypeDef f2;

int16 half_width[120] =
    {0, 0, 1, 1, 2, 2, 3, 3, 3, 4,
     4, 5, 5, 5, 6, 6, 7, 7, 8, 8,
     9, 9, 9, 10, 10, 11, 11, 12, 12, 12,
     12, 13, 13, 13, 13, 14, 14, 14, 14, 14,
     15, 15, 16, 16, 17, 18, 19, 20, 21, 21,
     21, 22, 23, 23, 24, 25, 25, 26, 27, 27,
     28, 29, 30, 31, 32, 33, 34, 35, 36, 36,
     37, 38, 39, 39, 40, 41, 41, 42, 43, 44,
     44, 45, 46, 47, 47, 48, 48, 49, 50, 50,
     51, 52, 53, 54, 54, 55, 56, 57, 58, 58,
     59, 60, 60, 61, 62, 63, 63, 64, 65, 65,
     66, 67, 67, 68, 69, 70, 70, 71, 72, 73};

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
    for (int j = MIN(f2.fork_R_row, f2.fork_L_row); j > 5; j--)
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
    }
  }

  if (f2.fork_up_line[f2.fork_up_col] < 40)
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
  find_R_edge_from_outter();
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
  find_L_edge_from_outter();
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

         f1.leftline[i] > f1.leftline[i - 3] &&
         f1.leftline[i] > f1.leftline[i + 6] &&
         f1.leftline[i] > f1.leftline[i - 5] &&

         (f1.leftlineflag[i - 5] == 0 ||
          f1.leftlineflag[i - 3] == 0 ||
          f1.leftlineflag[i - 10] == 0) &&

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

         (f1.rightlineflag[i - 5] == 0 ||
          f1.rightlineflag[i - 3] == 0 ||
          f1.rightlineflag[i - 10] == 0) &&

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
  f2.tag_top_row = 0;

  for (int i = 105; i > 10; i--)
  {
    if (abs(f1.tag_leftline[i] - f1.tag_leftline[i - 1]) > 8 ||
        abs(f1.tag_rightline[i] - f1.tag_rightline[i - 1]) > 8 ||
        (f1.tag_rightline[i] - f1.tag_leftline[i]) - 2 * half_width[i] > 20 ||
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
      { //Ѱ���������
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
      { //Ѱ���������
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

  disable_all_elements();
  f1.ramp_enable = 1;
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

  if (f1.ele_enable)
  {

#ifdef ENABLE_RAMP
    if (f1.ramp_enable && f1.en_ramp_det)
    {
      if (f2.ramp_sm_state == 0)
      {
        ramp_judge();

        if (f2.ramp_flag)
        {
          set_ramp_flag();
        }
      }

      //״̬1,��������
      if (f2.ramp_sm_state == 1)
      {
        if (
            fabs(inoutdev.gyro_y_angle) > 8 ||
            chassis.ramp_distance > 50)
        {
          f2.ramp_sm_state = 2;
          chassis.ramp_distance = 0;
        }
      }

      //״̬2,���¶�
      if (f2.ramp_sm_state == 2)
      {
        if (
            fabs(inoutdev.gyro_y_angle) <= 4 ||
            chassis.ramp_distance > 50)
        {
          f2.ramp_sm_state = 3;
          chassis.ramp_distance = 0;
        }
      }

      //״̬3,��������
      if (f2.ramp_sm_state == 3)
      {
        if (inoutdev.gyro_y_angle <= -8 ||
            chassis.ramp_distance > 50)
        {
          f2.ramp_sm_state = 4;
          chassis.ramp_distance = 0;
        }
      }

      //״̬4,�뿪�µ�
      if (f2.ramp_sm_state == 4)
      {
        if (fabs(inoutdev.gyro_y_angle) <= 4 ||
            chassis.ramp_distance > 50)
        {
          inoutdev.lbeep(1);
          f2.ramp_sm_state = 0;
          chassis.ramp_distance = 0;

          enable_all_elements();
        }
      }
    }
#endif

#ifdef ENABLE_START
    if (f1.start_enable && f2.en_start)
    {
      //״̬0,��ʼ��
      if (f2.start_sm_state == 0)
      {
        inoutdev.beep(1);

        chassis.start_distance = 0;
        chassis.all_distance = 0;
        inoutdev.start_angle = 0;
        f2.start_num++;

        f2.start_sm_state = 3;
        disable_all_elements();
        f1.start_enable = 1;
      }

      //״̬2,2
      if (f2.start_sm_state == 2)
      {
        chassis.start_distance = 0;
        f2.start_sm_state = 0;
        f2.en_start = 0;

        inoutdev.start_angle = 0;
        enable_all_elements();
        inoutdev.gyro_y_angle = 0;
      }

      //����2,3
      if (f2.start_sm_state == 3)
      {
        if (chassis.start_distance > 12)
        {
          f2.start_sm_state = 4;
          chassis.start_distance = 0;
        }
        for (int i = 118; i > 10; i--)
        {
          f1.midline[i] = 80;
        }
        f2.toppoint = 10;
        return;
      }

      //����2,4
      if (f2.start_sm_state == 4)
      {
        if (fabs(inoutdev.start_angle) > 50 ||
            chassis.start_distance > 60)
        {
          f2.start_sm_state = 2;
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

#ifdef ENABLE_QRCODE
    if (f1.tag_enable && f1.en_anifriut_regco)
    {
      //״̬0,�ж϶�ά��
      if (f2.tag_sm_state == 0)
      {
        qrcode_judge();
        if (f2.tag_flag)
        {
          set_qrcode_flag();
        }
      }

      //״̬1,��ά�벹��
      if (f2.tag_sm_state == 1)
      {
        find_qrcode_pos();
        qrcode_patch();
        return;
      }

      //״̬2,�˳���ά�벹��
      if (f2.tag_sm_state == 2)
      {
        f2.tag_sm_state = 0;

        inoutdev.lbeep(1);
        enable_all_elements();
        inoutdev.gyro_y_angle = 0;
        // chassis.all_distance = 0;
      }
    }
#endif

#ifdef ENABLE_GARAGE
    if (f1.garage_enable)
    {
      //״̬0,���������
      if (f2.garage_sm_state == 0)
      {
        garage_judge();

        if (f2.garage_flag &&
            f2.garage_frames >= 1)
        {
          set_garage_flag();
        }
      }

      //״̬4,�복��ǰ
      if (f2.garage_sm_state == 4)
      {
        // garage_judge();
        find_garage_row();
        if (f2.garage_start_row > 100 ||
            chassis.start_distance >= f1.garage_regco_dis - 5)
        {
          f2.garage_sm_state = 1;
          chassis.start_distance = 0;
        }
      }

      //״̬1,�������복��
      if (f2.garage_sm_state == 1)
      {
        if (fabs(inoutdev.start_angle) > 70 &&
            chassis.start_distance > 30)
        {
          inoutdev.beep(1);
          chassis.start_distance = 0;
          f2.garage_sm_state = 2;
          inoutdev.start_angle = 0;
          chassis.set_speed(0, 0);
          chassis_bake();
        }

        for (int i = 118; i > 10; i--)
        {
          if (f2.garage_dir == 0)
            f1.midline[i] = 0;
          else if (f2.garage_dir == 1)
            f1.midline[i] = 159;
        }
        return;
      }

      //״̬2,�ѽ��복��
      if (f2.garage_sm_state == 2)
      {
        for (int i = 118; i > 10; i--)
        {
          f1.midline[i] = 80;
        }
      }

      //״̬5,�����ܵڶ�Ȧ
      if (f2.garage_sm_state == 5)
      {
        if (chassis.start_distance > 230 ||
            chassis.start_distance >= f1.garage_regco_dis + 20)
        {
          inoutdev.beep(1);
          chassis.start_distance = 0;
          f2.garage_sm_state = 0;
          inoutdev.start_angle = 0;
          enable_all_elements();
          inoutdev.gyro_y_angle = 0;
        }

        continue_patch();
        return;
      }
    }
#endif

#ifdef ENABLE_FORK
    if (f1.fork_enable)
    {
      //״̬0,�������·��
      if (f2.fork_sm_state == 0)
      {
        fork_road_judge();
        if (f2.fork_flag)
        {
          set_fork_flag();
        }
      }

      //״̬1,����·�ڲ���
      if (f2.fork_sm_state == 1 ||
          f2.fork_sm_state == 2)
      {
        //��ת
        if ((f2.fork_num == 1 && f2.fork1_dir == 1) ||
            (f2.fork_num == 2 && f2.fork1_dir == 1) ||
            (f2.fork_num == 3 && f2.fork2_dir == 1) ||
            (f2.fork_num == 4 && f2.fork2_dir == 1) ||

            (f2.fork_num == 5 && f2.fork1_dir == 1) ||
            (f2.fork_num == 6 && f2.fork1_dir == 1) ||
            (f2.fork_num == 7 && f2.fork2_dir == 1) ||
            (f2.fork_num == 8 && f2.fork2_dir == 1))
        {
          R_fork_patch();
          fork_road_judge();

          //״̬��ת
          if (f2.fork_sm_state == 1)
            if (f1.leftstartrow < 75 &&
                chassis.fork_distance > 10)
            {
              f2.fork_sm_state = 2;
              chassis.fork_distance = 0;
            }

          //״̬��λ
          if (f2.fork_sm_state == 2 && !f2.fork_flag)
            if (chassis.fork_distance > 70)
            {
              inoutdev.beep(1);
              f2.fork_sm_state = 0;
              enable_all_elements();
              inoutdev.gyro_y_angle = 0;
            }
        }

        //��ת
        if ((f2.fork_num == 1 && f2.fork1_dir == 0) ||
            (f2.fork_num == 2 && f2.fork1_dir == 0) ||
            (f2.fork_num == 3 && f2.fork2_dir == 0) ||
            (f2.fork_num == 4 && f2.fork2_dir == 0) ||

            (f2.fork_num == 5 && f2.fork1_dir == 0) ||
            (f2.fork_num == 6 && f2.fork1_dir == 0) ||
            (f2.fork_num == 7 && f2.fork2_dir == 0) ||
            (f2.fork_num == 8 && f2.fork2_dir == 0))
        {
          L_fork_patch();
          fork_road_judge();

          //״̬��ת
          if (f2.fork_sm_state == 1)
            if (f1.rightstartrow < 75 &&
                chassis.fork_distance > 10)
            {
              f2.fork_sm_state = 2;
              chassis.fork_distance = 0;
            }

          //״̬��λ
          if (f2.fork_sm_state == 2 && !f2.fork_flag)
            if (chassis.fork_distance > 70)
            {
              inoutdev.beep(1);
              f2.fork_sm_state = 0;
              enable_all_elements();
              inoutdev.gyro_y_angle = 0;
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
        if (f2.L_loop_flag == 1)
        {
          set_L_loop_flag();
        }
      }

      if (f2.L_loop_sm_state == 0 && f2.R_loop_sm_state == 0)
      {
        R_loop_judge();
        if (f2.R_loop_flag == 1)
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

  find_post_proc();
}
