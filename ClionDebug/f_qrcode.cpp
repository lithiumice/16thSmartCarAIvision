#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void qrcode_judge()
{
    find_L_edge_from_outter();
    find_R_edge_from_outter();

    f2.tag_flag = 0;
    f2.tag_row_up = 0;
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

            f1.tag_black_duty[i + 0] < 60 &&
            f1.tag_black_duty[i - 1] < 60 &&
            f1.tag_black_duty[i - 2] < 60 &&
            f1.tag_black_duty[i - 3] < 60)
        {
            f2.tag_row_down = i;
            break;
        }
    }

    for (int i = f2.tag_row_down; i > f2.tag_top_row; i--)
    {
        if (
            (f1.tag_black_duty[i - 1] < f2.duty_threshold) &&

            f1.tag_black_duty[i + 0] > f2.duty_threshold &&
            f1.tag_black_duty[i + 1] > f2.duty_threshold &&
            f1.tag_black_duty[i + 2] > f2.duty_threshold &&
            f1.tag_black_duty[i + 3] > f2.duty_threshold &&

            f1.tag_black_duty[i + 0] < 60 &&
            f1.tag_black_duty[i + 1] < 60 &&
            f1.tag_black_duty[i + 2] < 60 &&
            f1.tag_black_duty[i + 3] < 60)
        {
            f2.tag_row_up = i;
            break;
        }
    }

    if (
        f2.tag_top_row > 10 &&
        f2.tag_top_row < 90 &&

        f2.tag_row_up &&
        f2.tag_row_down &&

        (f1.leftstartrow > 80 ||
         f1.rightstartrow > 80) &&

        is_continuous_line(f1.tag_L_start_row - 1,
                           f2.tag_row_up - 1,
                           f1.tag_leftline, 3) &&
        is_continuous_line(f1.tag_R_start_row - 1,
                           f2.tag_row_up - 1,
                           f1.tag_rightline, 3))
    {
        f2.tag_flag = 1;
        f2.tag_frames++;
    }
    else
    {
        f2.tag_flag = 0;
        f2.tag_frames = 0;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void qrcode_patch()
{
    find_mem_L_edge();
    find_mem_R_edge();

    f2.toppoint = 0;
    f2.R_mid_endrow = 0;
    f2.L_mid_endrow = 0;
    for (int i = 118; i > 10; i--)
    {
        if (f2.qrcode_num == 1 || f2.qrcode_num == 3)
            f1.midline[i] = (f1.tag_leftline[i] + f1.tag_rightline[i]) / 2 + 15;
        else
            f1.midline[i] = (f1.tag_leftline[i] + f1.tag_rightline[i]) / 2 + 6;

        if (f1.midline[i] <= 0)
        {
            f1.midline[i] = 0;
            f2.toppoint = i;
            f2.L_mid_endrow = i;
            break;
        }
        if (f1.midline[i] >= 159)
        {
            f1.midline[i] = 159;
            f2.toppoint = i;
            f2.R_mid_endrow = i;
            break;
        }
    }

    f2.toppoint = 20;
}
