#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void cross_judge()
{
    find_cross_infec();
    f2.cross_flag = 0;
    f2.cross_type = 0;

    if (f2.L_infec_flag == 1 &&
        f2.R_infec_flag == 1 &&
        f2.L_up_infec_flag == 1 &&
        f2.R_up_infec_flag == 1 &&

        f2.L_infec_row > f2.L_up_infec_row &&
        f2.R_infec_row > f2.R_up_infec_row &&

        f2.R_infec_col - f2.L_infec_col > 20 &&
        f2.R_up_infec_col - f2.L_up_infec_col > 10 &&
        f2.R_infec_col - f2.L_infec_col > f2.R_up_infec_col - f2.L_up_infec_col)
    {
        f2.cross_flag = 1;
        f2.cross_frames++;
        f2.cross_type = 0;
    }
    else if (
        f2.L_infec_flag == 1 &&
        f2.L_up_infec_flag == 1 &&
        f2.L_infec_row > f2.L_up_infec_row &&
        f2.L_up_infec_col < 140 &&
        f2.L_infec_col < f2.L_up_infec_col)
    {
        f2.cross_flag = 1;
        f2.cross_frames++;
        f2.cross_type = 1;
    }
    else if (
        f2.R_infec_flag == 1 &&
        f2.R_up_infec_flag == 1 &&
        f2.R_infec_row > f2.R_up_infec_row &&
        f2.R_up_infec_col > 20 &&
        f2.R_infec_col > f2.R_up_infec_col)
    {
        f2.cross_flag = 1;
        f2.cross_frames++;
        f2.cross_type = 2;
    }
    else if (
        f2.R_infec_flag == 0 &&
        f2.L_infec_flag == 0 &&
        f2.L_up_infec_flag == 1 &&
        f2.R_up_infec_flag == 1 &&

        f2.R_up_infec_row > 40 &&
        f2.L_up_infec_row > 40 &&

        f2.R_up_infec_col > 30 &&
        f2.L_up_infec_col < 140 &&

        f2.R_up_infec_col - f2.L_up_infec_col > 15)
    {
        f2.cross_flag = 1;
        f2.cross_frames++;
        f2.cross_type = 3;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void cross_patch()
{
    f2.cross_K_L = 0;
    f2.cross_K_R = 0;

    if (f2.cross_type == 0)
    {
        f2.cross_K_L = ((float)(f2.L_up_infec_col - f2.L_infec_col)) / (f2.L_infec_row - f2.L_up_infec_row);
        for (int i = f2.L_infec_row; i >= f2.L_up_infec_row; i--)
        {
            f1.leftline[i] = (int)(f2.L_infec_col + f2.cross_K_L * (f2.L_infec_row - i));
            __Limit_AB(f1.leftline[i], 0, 159);
        }

        f2.cross_K_R = ((float)(f2.R_infec_col - f2.R_up_infec_col)) / (f2.R_infec_row - f2.R_up_infec_row);
        for (int i = f2.R_infec_row; i >= f2.R_up_infec_row; i--)
        {
            f1.rightline[i] = (int)(f2.R_infec_col - f2.cross_K_R * (f2.R_infec_row - i));
            __Limit_AB(f1.rightline[i], 0, 159);
        }

        for (int i = MAX(f2.L_infec_row, f2.R_infec_row); i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
        {
            f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
            __Limit_AB(f1.midline[i], 0, 159);
        }
    }
    else if (f2.cross_type == 1)
    {
        f2.cross_K_L = ((float)(f2.L_up_infec_col - f2.L_infec_col)) / (f2.L_infec_row - f2.L_up_infec_row);
        for (int i = f2.L_infec_row; i >= f2.L_up_infec_row; i--)
        {
            f1.leftline[i] = (int)(f2.L_infec_col + f2.cross_K_L * (f2.L_infec_row - i));
            __Limit_AB(f1.leftline[i], 0, 159);
        }
        for (int i = f2.L_infec_row; i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
        {
            f1.midline[i] = f1.leftline[i] + half_width[i];
            __Limit_AB(f1.midline[i], 0, 159);
        }
    }
    else if (f2.cross_type == 2)
    {
        f2.cross_K_R = ((float)(f2.R_infec_col - f2.R_up_infec_col)) / (f2.R_infec_row - f2.R_up_infec_row);
        for (int i = f2.R_infec_row; i >= f2.R_up_infec_row; i--)
        {
            f1.rightline[i] = (int)(f2.R_infec_col - f2.cross_K_R * (f2.R_infec_row - i));
            __Limit_AB(f1.rightline[i], 0, 159);
        }
        for (int i = f2.R_infec_row; i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
        {
            f1.midline[i] = f1.rightline[i] - half_width[i];
            __Limit_AB(f1.midline[i], 0, 159);
        }
    }
    if (f2.cross_type == 3)
    {
        f2.cross_K_L = ((float)(f1.leftline[f2.L_up_infec_row - 8] - f2.L_up_infec_col)) / 8;
        for (int i = f2.L_up_infec_row; i <= 118; i++)
        {
            f1.leftline[i] = (int)(f2.L_up_infec_col + f2.cross_K_L * (f2.L_up_infec_row - i));
            __Limit_AB(f1.leftline[i], 0, 159);
        }

        f2.cross_K_R = ((float)(f2.R_up_infec_col - f1.rightline[f2.R_up_infec_row - 8])) / 8;
        for (int i = f2.R_up_infec_row; i <= 118; i++)
        {
            f1.rightline[i] = (int)(f2.R_up_infec_col - f2.cross_K_R * (f2.R_up_infec_row - i));
            __Limit_AB(f1.rightline[i], 0, 159);
        }

        for (int i = MIN(f2.R_up_infec_row, f2.L_up_infec_row); i <= 118; i++)
        {
            f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
            __Limit_AB(f1.midline[i], 0, 159);
        }
    }
}
