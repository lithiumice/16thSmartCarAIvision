#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void garage_judge()
{
    f2.garage_row = 0;
    f2.garage_tmp[0] = 0;
    f2.garage_tmp[1] = 0;
    f2.garage_start_row = 0;
    for (int16 i = 110; i >= 45; i--)
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
                f2.garage_tmp[1] - f2.garage_tmp[0] >= 1 &&
                f2.garage_tmp[1] - f2.garage_tmp[0] <= 10)
            {
                f2.garage_tmp[0] = 0;
                f2.garage_tmp[1] = 0;

                f2.garage_block_cnt++;
            }

            if (f2.garage_block_cnt >= 4 && //�᷽��,�ܹ�9��
                f2.garage_block_cnt <= 18)
            {

                if (!f2.garage_start_row)
                    f2.garage_start_row = i;

                if (abs(f2.garage_start_row - i) > 20)
                    continue;

                f2.garage_row++;
                f2.garage_block_cnt = 0;
                break;
            }
        }
        f2.garage_block_cnt = 0;
    }

    if (f2.garage_row >= 3 &&
        ((f2.L_infec_flag
          // f2.L_infec_row - f2.garage_start_row > 0 &&
          // f2.L_infec_row - f2.garage_start_row < 20 &&
          && f2.garage_dir == 0
          // is_straight_line(f2.L_infec_row,f1.rightstartrow-1,f1.rightline,4)
          ) ||

         (f2.R_infec_flag
          // f2.R_infec_row - f2.garage_start_row > 0 &&
          // f2.R_infec_row - f2.garage_start_row < 20 &&
          && f2.garage_dir == 1
          //  is_straight_line(f2.R_infec_row,f1.leftstartrow-1,f1.leftline,4)
          )))
    {
        f2.garage_flag = 1;
        f2.garage_frames++;
    }
    else
    {
        f2.garage_flag = 0;
        f2.garage_frames = 0;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void continue_patch()
{
    f2.L_mid_endrow = 0;
    f2.R_mid_endrow = 0;

    if (f2.garage_dir == 1)
    {
        f1.midline[118] = 80;
        for (int i = 117; i > 10; i--)
        {
            f1.midline[i] = 80;
            f1.leftline[i] = 0;
            f1.leftlineflag[i] = 0;

            for (int j = 2; j < 140; j++)
            {
                if (IMG_120_160[i][j] != 0 &&
                    IMG_120_160[i][j - 1] != 0 &&
                    IMG_120_160[i][j - 2] == 0)
                {
                    f1.leftline[i] = j - 1;
                    f1.leftlineflag[i] = 1;
                    break;
                }
            }

            f1.midline[i] = f1.leftline[i] + half_width[i];
            if (i < 90 && abs(f1.midline[i] - f1.midline[i + 1]) > 15)
                f1.midline[i] = f1.midline[i + 1];

            R_MID_BREAK
        }
        f2.toppoint = 20;
        R_mid_endrow_proc();
    }

    if (f2.garage_dir == 0)
    {
        f1.midline[118] = 80;
        for (int i = 118; i > 10; i--)
        {
            f1.midline[i] = 80;
            f1.rightline[i] = 159;
            f1.rightlineflag[i] = 0;

            for (int j = 158; j > 20; j--)
            {
                if (IMG_120_160[i][j] != 0 &&
                    IMG_120_160[i][j + 1] != 0 &&
                    IMG_120_160[i][j + 2] == 0)
                {
                    f1.rightline[i] = j + 1;
                    f1.rightlineflag[i] = 1;
                    break;
                }
            }

            f1.midline[i] = f1.rightline[i] - half_width[i];
            if (i < 90 && abs(f1.midline[i] - f1.midline[i + 1]) > 15)
                f1.midline[i] = f1.midline[i + 1];

            L_MID_BREAK
        }
        f2.toppoint = 20;
        L_mid_endrow_proc();
    }
}
