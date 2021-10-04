#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void find_little_curve()
{

    f2.leftturnflag = 0;
    f2.leftturn_row = 0;
    f2.rightturnflag = 0;
    f2.rightturn_row = 0;

    for (int i = 115; i > f2.toppoint + 3; i--)
    {
        //����
        if (f1.leftlineflag[i] == 0 &&
            f2.leftturnflag == 0 &&
            f1.leftlineflag[i + 1] == 1 &&
            f1.leftlineflag[i + 2] == 1 &&

            f1.rightlineflag[i] == 1 &&
            f1.rightlineflag[i - 5] == 1 &&
            i > 20 &&
            i < 100)
        {
            f2.leftturn_row = i;
            f2.leftturnflag = 1;
            if (f2.leftturn_row < 60 &&
                f1.rightline[f2.leftturn_row - 5] > 100 &&
                f1.rightline[f2.leftturn_row - 5] > f1.rightline[f2.leftturn_row] &&
                f1.rightline[f2.leftturn_row - 5] - f1.rightline[f2.leftturn_row] > 10)
            {
                f2.leftturn_row = 0;
                f2.leftturnflag = 0;
            }

            //ʮ���ų�
            int lose_cnt = 0;
            if (f2.cross_flag == 1) //��ֹʮ�ָ�С����жϴ�����Ӱ��
            {
                for (int j = f2.leftturn_row; j > f2.toppoint; j--)
                    if (f1.rightlineflag[j] == 0)
                        lose_cnt++;
                if (lose_cnt > 5)
                {
                    f2.leftturn_row = 0;
                    f2.leftturnflag = 0;
                }
            }
            break;
        }

        //����
        if (f1.rightlineflag[i] == 0 &&
            f2.rightturnflag == 0 &&
            f1.rightlineflag[i + 1] == 1 &&
            f1.rightlineflag[i + 2] == 1 &&
            f1.leftlineflag[i] == 1 &&
            f1.leftlineflag[i - 5] == 1 &&
            i > 20 &&
            i < 100)
        {
            f2.rightturn_row = i;
            f2.rightturnflag = 1;
            if (f2.rightturn_row <= 60 &&
                f1.leftline[f2.rightturn_row - 5] < 60 &&
                f1.leftline[f2.rightturn_row - 5] < f1.leftline[f2.rightturn_row] &&
                f1.leftline[f2.rightturn_row] - f1.leftline[f2.rightturn_row - 5] > 10)
            {
                f2.rightturn_row = 0;
                f2.rightturnflag = 0;
            }

            //�ų�ʮ��
            int lose_cnt = 0;
            if (f2.cross_flag == 1)
            {
                for (int j = f2.rightturn_row; j > f2.toppoint; j--)
                    if (f1.leftlineflag[j] == 0)
                        lose_cnt++;
                if (lose_cnt > 5)
                {
                    f2.rightturn_row = 0;
                    f2.rightturnflag = 0;
                }
            }
            break;
        }
    }
}

void find_large_curve()
{
    find_toppoint();

    if (
        is_L_R_nonlose(f1.rightstartrow, f2.toppoint, 1, 95) &&
        is_L_R_lose(118, f2.toppoint, 0, 80))
    {
        f2.leftturnflag = 3;
    }

    if (
        is_L_R_nonlose(f1.leftstartrow, f2.toppoint, 0, 95) &&
        is_L_R_lose(118, f2.toppoint, 1, 80))
    {
        f2.rightturnflag = 3;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void curve_patch()
{
    //С�䲹��
    if (f2.leftturnflag == 1)
    {
        for (int i = f2.leftturn_row; i > f2.toppoint; i--)
        {
            if (f1.rightline[i] == 159)
            {
                f1.midline[i] = f1.midline[i + 1] - 2;
            }
            else
            {
                f1.midline[i] = f1.midline[i + 1] - (f1.rightline[i + 1] - f1.rightline[i] + 1);
            }
            L_MID_BREAK
        }
        L_mid_endrow_proc();
    }

    //���䲹��
    if (f2.leftturnflag == 3)
    {
        for (int i = 118; i > 0; i--)
        {
            f1.midline[i] = f1.rightline[i] - 80;
            __Limit_AB(f1.midline[i], 0, 159);
            L_MID_BREAK
        }
        L_mid_endrow_proc();
    }

    //С�䲹��
    if (f2.rightturnflag == 1)
    {
        for (int i = f2.rightturn_row; i > 0; i--)
        {
            if (f1.leftline[i] == 0)
            {
                f1.midline[i] = f1.midline[i] + 2;
            }
            else
            {
                f1.midline[i] = f1.midline[i + 1] + (f1.leftline[i] - f1.leftline[i + 1] + 1);
            }

            R_MID_BREAK
        }
        R_mid_endrow_proc();
    }

    //���䲹��
    if (f2.rightturnflag == 3)
    {
        for (int i = 118; i > 0; i--)
        {
            f1.midline[i] = f1.leftline[i] + 80;
            __Limit_AB(f1.midline[i], 0, 159);
            R_MID_BREAK
        }
        R_mid_endrow_proc();
    }
}
