#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void fork_road_judge()
{
    find_up_infec();

    int fork_L_lose_row = 118;
    int fork_R_lose_row = 118;
    for (int i = 118; i > 30; i--)
    {
        if (f1.leftlineflag[i] == 0)
        {
            fork_L_lose_row = i;
            break;
        }
    }
    for (int i = 118; i > 30; i--)
    {
        if (f1.rightlineflag[i] == 0)
        {
            fork_R_lose_row = i;
            break;
        }
    }

    // if (
    //     (f2.fork_L_flag == 1 &&
    //      f2.fork_R_flag == 1 &&
    //      f2.fork_R_col - f2.fork_L_col > 20 &&
    //      abs(f2.fork_R_row - f2.fork_L_row) < 15 &&
    //      f2.toprow_80col > 45 &&
    //      MIN(fork_L_lose_row, fork_R_lose_row) - f2.toprow_80col > 15 &&
    //      is_L_R_lose(MIN(fork_L_lose_row, fork_R_lose_row), f2.toppoint, 2, 90)) ||

    //     (abs(((f2.fork_R_col + f2.fork_L_col) / 2) - f2.fork_up_col) <= 10 &&
    //      //   is_straight_line(f2.fork_up_col - 20, f2.fork_up_col, f2.fork_up_line, 5) &&
    //      //   is_straight_line(f2.fork_up_col, f2.fork_up_col + 20, f2.fork_up_line, 5) &&
    //      abs(f2.fork_R_row - f2.fork_L_row) < 20 &&
    //      f2.fork_up_flag == 1 &&
    //      f2.fork_L_flag == 1 &&
    //      f2.fork_R_flag == 1 &&
    //      f2.fork_R_col > f2.fork_L_col &&
    //      f2.fork_up_row < f2.fork_L_row &&
    //      f2.fork_up_row < f2.fork_R_row) ||

    //     (f2.fork_up_flag == 1 &&
    //      f2.fork_up_row > 55 &&
    //      abs((f2.fork_up_line[f2.fork_up_col - 15] - f2.fork_up_line[f2.fork_up_col]) -
    //          (f2.fork_up_line[f2.fork_up_col + 15] - f2.fork_up_line[f2.fork_up_col])) <=4 &&
    //     //  is_straight_line(f2.fork_up_col - 30, f2.fork_up_col, f2.fork_up_line, 4) &&
    //     //  is_straight_line(f2.fork_up_col, f2.fork_up_col + 30, f2.fork_up_line, 4) &&
    //      is_L_R_lose(118, 70, 2, 90)) ||

    //     (f2.fork_L_flag == 0 &&
    //      f2.fork_R_flag == 1 &&
    //      f2.fork_up_flag == 1 &&
    //      f2.fork_up_col < 80 &&

    //     //  is_straight_line(5, f2.fork_up_col-3, f2.fork_up_line, 6) &&
    //     //  is_straight_line(f2.fork_up_col + 3, f2.fork_R_col-3, f2.fork_up_line, 6) &&

    //      f2.fork_up_line[10] > 25 &&
    //      f2.fork_up_line[10] < 70 &&
    //      f2.fork_R_col - f2.fork_up_col > 30 &&
    //      f2.fork_R_row - f2.fork_up_row > 20) ||

    //     (f2.fork_L_flag == 1 &&
    //      f2.fork_R_flag == 0 &&
    //      f2.fork_up_flag == 1 &&
    //      f2.fork_up_col > 80 &&

    //     //  is_straight_line(f2.fork_up_col+3, 155, f2.fork_up_line, 6) &&
    //     //  is_straight_line(f2.fork_up_col-3, f2.fork_L_col+3, f2.fork_up_line, 6) &&

    //      f2.fork_up_line[150] > 25 &&
    //      f2.fork_up_line[150] < 70 &&
    //      f2.fork_up_col - f2.fork_L_col > 30 &&
    //      f2.fork_L_row - f2.fork_up_row > 20))
     if (
        (f2.fork_L_flag == 1 &&
         f2.fork_R_flag == 1 &&
         f2.fork_R_col - f2.fork_L_col > 20 &&
         abs(f2.fork_R_row - f2.fork_L_row) < 15 &&
         f2.toprow_80col > 45 &&
         MIN(fork_L_lose_row, fork_R_lose_row) - f2.toprow_80col > 15 &&
         is_L_R_lose(MIN(fork_L_lose_row, fork_R_lose_row), f2.toppoint, 2, 90)) ||

        (abs(((f2.fork_R_col + f2.fork_L_col) / 2) - f2.fork_up_col) <= 10 &&
         //   is_straight_line(f2.fork_up_col - 20, f2.fork_up_col, f2.fork_up_line, 5) &&
         //   is_straight_line(f2.fork_up_col, f2.fork_up_col + 20, f2.fork_up_line, 5) &&
         abs(f2.fork_R_row - f2.fork_L_row) < 20 &&
         f2.fork_up_flag == 1 &&
         f2.fork_L_flag == 1 &&
         f2.fork_R_flag == 1 &&
         f2.fork_R_col > f2.fork_L_col &&
         f2.fork_up_row < f2.fork_L_row &&
         f2.fork_up_row < f2.fork_R_row) ||

        (f2.fork_up_flag == 1 &&
         f2.fork_up_row > 55 &&
         abs((f2.fork_up_line[f2.fork_up_col - 15] - f2.fork_up_line[f2.fork_up_col]) -
             (f2.fork_up_line[f2.fork_up_col + 15] - f2.fork_up_line[f2.fork_up_col])) <=4 &&
        //  is_straight_line(f2.fork_up_col - 30, f2.fork_up_col, f2.fork_up_line, 4) &&
        //  is_straight_line(f2.fork_up_col, f2.fork_up_col + 30, f2.fork_up_line, 4) &&
         is_L_R_lose(118, 70, 2, 90)) ||

        (f2.fork_L_flag == 0 &&
         f2.fork_up_flag == 1 &&
         f2.fork_R_flag == 1 &&
         f2.fork_up_col <= 83 &&
         f2.fork_up_row>20

        // //  is_straight_line(5, f2.fork_up_col-3, f2.fork_up_line, 6) &&
        // //  is_straight_line(f2.fork_up_col + 3, f2.fork_R_col-3, f2.fork_up_line, 6) &&

        //  f2.fork_up_line[10] > 15 &&
        //  f2.fork_up_line[10] < 90 &&
        //  f2.fork_R_col - f2.fork_up_col > 10 &&
        //  f2.fork_R_row - f2.fork_up_row > 10
         ) ||

        (f2.fork_L_flag == 1 &&
         f2.fork_R_flag == 0 &&
         f2.fork_up_flag == 1 &&
         f2.fork_up_col > 80 &&

        //  is_straight_line(f2.fork_up_col+3, 155, f2.fork_up_line, 6) &&
        //  is_straight_line(f2.fork_up_col-3, f2.fork_L_col+3, f2.fork_up_line, 6) &&

         f2.fork_up_line[150] > 15 &&
         f2.fork_up_line[150] < 80 &&
         f2.fork_up_col - f2.fork_L_col > 10 &&
         f2.fork_L_row - f2.fork_up_row > 10))
    {
        f2.fork_flag = 1;
        f2.fork_frames++;
    }
    else
    {
        f2.fork_flag = 0;
        f2.fork_frames = 0;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void L_fork_patch()
{
    f2.L_fork_mid_endrow = f2.toppoint;

    for (int i = 115; i > 10; i--)
    {
        if (f1.leftlineflag[i] == 1 || i > 110)
        {
            f1.midline[i] = f1.leftline[i] + half_width[i];
        }
        else
        {
            f1.midline[i] = f1.midline[i + 1] - 2;
        }

        if (abs(f1.midline[i] - f1.midline[i + 1]) > 15)
            f1.midline[i] = f1.midline[i + 1];

        if (f1.midline[i] <= 0)
        {
            f1.midline[i] = 0;
            f2.L_fork_mid_endrow = i;
            break;
        }
    }

    for (int i = f2.L_fork_mid_endrow; i > 0; i--)
    {
        f1.leftline[i] = 0;
        f1.rightline[i] = 0;
        f1.midline[i] = 0;
    }
}

void R_fork_patch()
{
    f2.R_fork_mid_endrow = f2.toppoint;

    for (int i = 115; i > 10; i--)
    {
        if (f1.rightlineflag[i] == 1 || i > 110)
        {
            f1.midline[i] = f1.rightline[i] - half_width[i];
        }
        else
        {
            f1.midline[i] = f1.midline[i + 1] + 2;
        }

        if (abs(f1.midline[i] - f1.midline[i + 1]) > 15)
            f1.midline[i] = f1.midline[i + 1];

        if (f1.midline[i] >= 159)
        {
            f1.midline[i] = 159;
            f2.R_fork_mid_endrow = i;
            break;
        }
    }

    for (int i = f2.R_fork_mid_endrow; i > 0; i--)
    {
        f1.leftline[i] = 159;
        f1.rightline[i] = 159;
        f1.midline[i] = 159;
    }
}
