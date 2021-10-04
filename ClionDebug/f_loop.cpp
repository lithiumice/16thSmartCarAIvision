#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void L_loop_judge()
{
    f2.L_loop_circle_row = 0;
    f2.L_loop_circle_col = 0;
    f2.L_loop_down_row = 0;
    f2.L_loop_up_row = 0;
    f2.L_loop_top_row = 0;

    find_R_edge_from_outter();
    find_L_egde_from_R();
    find_topstart_row();
    find_R_tag_toppoint();

    for (int i = 100; i > f2.toppoint; i--)
    {
        if (
            !f1.leftlineflag[i + 20] &&
            !f1.leftlineflag[i + 10] &&
            !f1.leftlineflag[i + 6] &&
            !f1.leftlineflag[i + 4] &&
            !f1.leftlineflag[i + 2] &&
            f1.leftlineflag[i] &&
            f1.leftlineflag[i - 2] &&
            f1.leftlineflag[i - 4] &&
            f1.leftlineflag[i - 6]

            && f1.leftline[i - 6] - f1.leftline[i] > 2)
        {
            f2.L_loop_down_row = i;
            break;
        }
    }

    for (int i = f2.L_loop_down_row; i > f2.toppoint; i--)
    {
        if (
            f1.leftline[i] > f1.leftline[i - 4] &&
            f1.leftline[i] > f1.leftline[i + 4] &&
            f1.leftline[i] > f1.leftline[i + 6] &&

            f1.leftlineflag[i - 4] &&
            f1.leftlineflag[i] &&
            f1.leftlineflag[i + 4] &&
            f1.leftlineflag[i + 6])
        {
            f2.L_loop_circle_row = i;
            break;
        }
    }

    for (int i = f2.L_loop_circle_row; i > 18; i--)
    {
        if (
            f1.tag_leftlineflag[i + 4] &&
            f1.tag_leftlineflag[i + 2] &&
            f1.tag_leftline[i + 2] - f1.tag_leftline[i] > 20)
        {
            f2.L_loop_up_row = i;
            break;
        }
    }

    for (int i = f2.L_loop_up_row; i > f1.tag_toppoint; i--)
    {
        if (
            f1.tag_leftlineflag[i - 4] &&
            f1.tag_leftlineflag[i - 2] &&
            f1.tag_leftlineflag[i] &&

            abs(f1.tag_leftline[i] - f1.tag_leftline[i - 2]) < 4 &&
            abs(f1.tag_leftline[i - 2] - f1.tag_leftline[i - 4]) < 4 &&

            f1.tag_leftline[i] - f1.tag_leftline[i + 2] > 20)

        {
            f2.L_loop_top_row = i;
            break;
        }
    }

    if ((f1.rightstartrow >= 118 ||

         (f1.rightstartrow > 85 &&
          f1.rightstartrow < 118 &&
          f1.rightline[f1.rightstartrow] > 155)) &&

        f2.L_loop_circle_row < 80 &&

        is_L_R_nonlose(f2.L_loop_circle_row,
                       f1.rightstartrow,
                       1,
                       95) &&
        is_straight_line(f2.L_loop_top_row,
                         f1.tag_R_start_row,
                         f1.tag_rightline, 5))
    {
        f2.L_loop_flag = 1;
    }
    else
    {
        f2.L_loop_flag = 0;
    }
}

void R_loop_judge()
{
    f2.R_loop_down_row = 0;
    f2.R_loop_circle_row = 0;
    f2.R_loop_up_row = 0;
    f2.R_loop_top_row = 0;

    find_L_edge_from_outter();
    find_R_egde_from_L();
    find_topstart_row();
    find_L_tag_toppoint();

    for (int i = 100; i > f2.toppoint; i--)
    {
        if (
            !f1.rightlineflag[i + 20] &&
            !f1.rightlineflag[i + 10] &&
            !f1.rightlineflag[i + 2] &&
            f1.rightlineflag[i] &&
            f1.rightlineflag[i - 2] &&
            f1.rightlineflag[i - 4] &&
            abs(f1.rightline[i - 6] - f1.rightline[i]) >= 2)
        {
            f2.R_loop_down_row = i;
            break;
        }
    }

    for (int i = f2.R_loop_down_row; i > f2.toppoint; i--)
    {
        if (
            f1.rightline[i] < f1.rightline[i - 4] &&
            f1.rightline[i] < f1.rightline[i + 4] &&
            f1.rightline[i] < f1.rightline[i + 6] &&

            f1.rightlineflag[i - 4] &&
            f1.rightlineflag[i] &&
            f1.rightlineflag[i + 4] &&
            f1.rightlineflag[i + 6])
        {
            f2.R_loop_circle_row = i;
            break;
        }
    }

    for (int i = f2.R_loop_circle_row; i > f1.tag_toppoint; i--)
    {
        if (
            f1.tag_rightlineflag[i + 4] &&
            f1.tag_rightlineflag[i + 2] &&
            f1.tag_rightline[i] - f1.tag_rightline[i + 2] > 10)
        {
            f2.R_loop_up_row = i;
            break;
        }
    }

    for (int i = f2.R_loop_up_row; i > f1.tag_toppoint; i--)
    {
        if (
            f1.tag_rightlineflag[i - 4] &&
            f1.tag_rightlineflag[i - 2] &&
            f1.tag_rightlineflag[i] &&

            abs(f1.tag_rightline[i] - f1.tag_rightline[i - 2]) <= 3 &&
            abs(f1.tag_rightline[i - 2] - f1.tag_rightline[i - 4]) <= 3 &&

            f1.tag_rightline[i + 2] - f1.tag_rightline[i] > 20)
        {
            f2.R_loop_top_row = i;
            break;
        }
    }

    if ((f1.leftstartrow >= 118 ||
         (f1.leftstartrow > 85 &&
          f1.leftstartrow < 118 &&
          f1.leftline[f1.leftstartrow] <= 3)) &&

        f2.R_loop_circle_row < 80 &&

        is_L_R_nonlose(f2.R_loop_circle_row,
                       f1.leftstartrow,
                       0, 95) &&

        is_straight_line(f2.R_loop_top_row,
                         f1.tag_L_start_row,
                         f1.tag_leftline, 5))
    {
        f2.R_loop_flag = 1;
    }
    else
    {
        f2.R_loop_flag = 0;
    }
}

// void R_loop_judge()
// {
//     f2.R_loop_flag = 0;

//     //step1
//     if (f2.R_loop_judge_sm == 0)
//     {
//         f2.R_loop_down_row = 0;

//         for (int i = 110; i > f2.toppoint; i--)
//         {
//             if (
//                 !f1.rightlineflag[i + 4] &&
//                 !f1.rightlineflag[i + 2] &&
//                 f1.rightlineflag[i] &&
//                 f1.rightlineflag[i - 2] &&
//                 f1.rightlineflag[i - 4] &&
//                 abs(f1.rightline[i - 6] - f1.rightline[i]) >= 2) //157-153
//             {
//                 f2.R_loop_down_row = i;
//                 f2.R_loop_judge_sm = 1;
//                 break;
//             }
//         }
//     }

//     //step2
//     if (f2.R_loop_judge_sm == 1)
//     {

//         find_L_edge();
//         find_R_egde_from_L();
//         find_topstart_row();
//         find_L_tag_toppoint();

//         f2.R_loop_circle_flag = 0;
//         f2.R_loop_circle_row = 0;
//         f2.R_loop_circle_col = 0;
//         for (int i = f2.R_loop_down_row; i > f2.toppoint; i--)
//         {
//             if (
//                 f1.rightline[i] < f1.rightline[i - 4] &&
//                 f1.rightline[i] < f1.rightline[i + 4] &&
//                 f1.rightline[i] < f1.rightline[i + 6] &&

//                 f1.rightlineflag[i - 4] &&
//                 f1.rightlineflag[i] &&
//                 f1.rightlineflag[i + 4] &&
//                 f1.rightlineflag[i + 6])
//             {
//                 f2.R_loop_circle_flag = 1;
//                 f2.R_loop_circle_row = i;
//                 f2.R_loop_circle_col = f1.rightline[i];
//                 break;
//             }
//         }

//         f2.R_loop_judge_sm = 0;

//         if (
//             (f1.leftstartrow >= 118 ||
//              (f1.leftstartrow > 85 &&
//               f1.leftstartrow < 118 &&
//               f1.leftline[f1.leftstartrow] <= 3)) &&

//             f2.R_loop_circle_flag &&
//             f2.R_loop_circle_row < 80 &&

//             is_L_R_nonlose(f2.R_loop_circle_row,
//                            f1.leftstartrow,
//                            0,
//                            95))
//         {
//             f2.R_loop_judge_sm = 2;
//         }
//     }

//     //step3
//     if (f2.R_loop_judge_sm == 2)
//     {
//         f2.R_loop_judge_sm = 0;
//         f2.R_loop_up_row = 0;

//         for (int i = f2.R_loop_circle_row; i > f1.tag_toppoint; i--)
//         {
//             if (
//                 f1.tag_rightlineflag[i + 4] &&
//                 f1.tag_rightlineflag[i + 2] &&
//                 f1.tag_rightline[i] - f1.tag_rightline[i + 2] > 10)
//             {
//                 f2.R_loop_up_row = i;
//                 f2.R_loop_judge_sm = 3;
//                 break;
//             }
//         }
//     }

//     //step4
//     if (f2.R_loop_judge_sm == 3)
//     {
//         f2.R_loop_judge_sm = 0;
//         f2.R_loop_top_row = 0;

//         for (int i = f2.R_loop_up_row; i > f1.tag_toppoint; i--)
//         {
//             if (
//                 f1.tag_rightlineflag[i - 4] &&
//                 f1.tag_rightlineflag[i - 2] &&
//                 f1.tag_rightlineflag[i] &&

//                 abs(f1.tag_rightline[i] - f1.tag_rightline[i - 2]) <= 3 &&
//                 abs(f1.tag_rightline[i - 2] - f1.tag_rightline[i - 4]) <= 3 &&

//                 f1.tag_rightline[i + 2] - f1.tag_rightline[i] > 20)
//             {
//                 f2.R_loop_top_row = i;
//                 if (is_straight_line(f2.R_loop_top_row,
//                                      f1.tag_L_start_row,
//                                      f1.tag_leftline, 5))
//                 {
//                     f2.R_loop_flag = 1;
//                     f2.R_loop_judge_sm = 0;
//                 }
//                 break;
//             }
//         }
//     }
// }

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void L_loop_patch()
{

    if (f2.L_loop_sm_state == 1)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 2;
        }

        L_mid_endrow_proc();

        if (f1.leftstartrow < 73 &&
            chassis.loop_distance >= 25)
        {
            f2.L_loop_sm_state = 3;
            chassis.loop_distance = 0;
        }
    }

    //�뻷ǰ��״̬3
    if (f2.L_loop_sm_state == 3)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 2;
        }

        L_mid_endrow_proc();

        if (f1.leftstartrow > 105 &&
            chassis.loop_distance >= 10)
        {
            f2.L_loop_sm_state = 2;
            chassis.loop_distance = 0;
        }
    }

    //�뻷ǰ��״̬2
    if (f2.L_loop_sm_state == 2)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 2;
            }
            else if (i >= 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 1;
            }
            else
            {
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) - 10;
            }

            if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
                f1.midline[i] = f1.midline[i + 1];

            L_MID_BREAK
        }

        L_mid_endrow_proc();
        if (f1.rightstartrow < 75)
            f2.L_loop_sm_state = 4;
    }

    //�뻷״̬4
    if (f2.L_loop_sm_state == 4)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 2;
            }
            else if (i >= 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 1;
            }
            else
            {
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) - 10;
            }

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (f1.rightstartrow > 110)
            f2.L_loop_sm_state = 5;
    }

    //����״̬5
    if (f2.L_loop_sm_state == 5)
    {
        for (int i = 110; i > 35; i--)
        {
            f1.midline[i] = f1.rightline[i] - (int16)(loop_half_width[i]) + 2;

            if (i < 90 && abs(f1.midline[i] - f1.midline[i + 1]) > 10)
                f1.midline[i] = f1.midline[i + 1];

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 255)
            f2.L_loop_sm_state = 6;
    }

    //����״̬6
    if (f2.L_loop_sm_state == 6)
    {
        for (int i = 118; i > 35; i--)
        {
            if (i == 118 || f1.leftlineflag[i])
            {
                f1.midline[i] = f1.leftline[i] + half_width[i] + 5;
            }
            else
            {
                f1.midline[i] = f1.midline[i + 1] - 1;
            }

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 335)
        {
            chassis.loop_distance = 0;
            f2.L_loop_sm_state = 7;
        }
    }

    //����״̬7
    if (f2.L_loop_sm_state == 7)
    {
        for (int i = 110; i > f2.toppoint; i--)
        {
            if (i < 85 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 2;
            }
            else if (i >= 85 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 1;
            }
            else if (f1.rightline[i] > f1.rightline[i + 1] && f1.rightlineflag[i] != 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 1;
            }
            else
            {
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
            }

            L_MID_BREAK
        }

        L_mid_endrow_proc();
        if ((is_L_R_nonlose(75, 110, 2, 95) &&
             chassis.loop_distance > 70) ||

            chassis.loop_distance > 200)
        {
            inoutdev.beep(2);
            f2.L_loop_sm_state = 0;
            enable_all_elements();
            // chassis.all_distance = 0;
            inoutdev.gyro_y_angle = 0;
            inoutdev.cross_z_angle = 0;
        }
    }
}

void R_loop_patch()
{
    //�뻷ǰ��״̬1
    if (f2.R_loop_sm_state == 1)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.leftlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 8;

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (f1.rightstartrow < 77 &&
            chassis.loop_distance >= 10)
        {
            f2.R_loop_sm_state = 3;
            chassis.loop_distance = 0;
        }
    }

    //�뻷ǰ��״̬3
    if (f2.R_loop_sm_state == 3)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.leftlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 8;

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (f1.rightstartrow > 100 &&
            chassis.loop_distance >= 10)
        {
            f2.R_loop_sm_state = 2;
            chassis.loop_distance = 0;
        }
    }

    //�뻷ǰ��״̬2
    if (f2.R_loop_sm_state == 2)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 2;
            }
            else if (i >= 108 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 1;
            }
            else
            {
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) + 8;
            }

            if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
                f1.midline[i] = f1.midline[i + 1];

            R_MID_BREAK
        }

        R_mid_endrow_proc();
        if (is_L_R_lose(90, 118, 0, 90))
            f2.R_loop_sm_state = 4;
    }

    //�뻷״̬4
    if (f2.R_loop_sm_state == 4)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 2;
            }
            else if (i >= 108 && f1.rightlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 1;
            }
            else
            {
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) + 8;
            }

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (is_L_R_nonlose(80, 110, 0, 80))
            f2.R_loop_sm_state = 5;
    }

    //����״̬5
    if (f2.R_loop_sm_state == 5)
    {
        for (int i = 110; i > 35; i--)
        {
            //TODO ���ڲ���
            f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]) + 6;

            if (i < 90 && f1.midline[i] < f1.midline[i + 1])
                f1.midline[i] = f1.midline[i + 1] + 1;

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 270)
            f2.R_loop_sm_state = 6;
    }

    //����״̬6
    if (f2.R_loop_sm_state == 6)
    {
        for (int i = 118; i > 35; i--)
        {
            if (i == 118 || f1.rightlineflag[i])
            {
                f1.midline[i] = f1.rightline[i] - half_width[i] - 5;
            }
            else
            {
                f1.midline[i] = f1.midline[i + 1] + 1;
            }

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 335)
        {
            f2.R_loop_sm_state = 7;
            chassis.loop_distance = 0;
        }
    }

    //����״̬7
    if (f2.R_loop_sm_state == 7)
    {
        for (int i = 110; i > f2.toppoint; i--)
        {
            if (i < 85 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 2;
            }
            else if (i >= 85 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] + 1;
            }
            else if (f1.leftline[i] < f1.leftline[i + 1] && f1.leftlineflag[i] != 0)
            {
                //��ֹ�����ͷ
                f1.midline[i] = f1.midline[i + 1] - 1;
            }
            else
            {
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 8;
            }

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if ((is_L_R_nonlose(75, 110, 2, 80) &&
             chassis.loop_distance > 70) ||

            chassis.loop_distance > 200)
        {
            inoutdev.beep(2);
            f2.R_loop_sm_state = 0;
            enable_all_elements();
            // chassis.all_distance = 0;
            inoutdev.gyro_y_angle = 0;
            inoutdev.cross_z_angle = 0;
        }
    }
}
