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

    find_R_edge_from_inner();
    find_L_egde_from_R();
    find_topstart_row();
    find_R_tag_toppoint();

    for (int i = 100; i > f2.toppoint; i--)
    {
        if (
            !f1.leftlineflag[i + 15] &&
            !f1.leftlineflag[i + 10] &&
            !f1.leftlineflag[i + 4] &&
            !f1.leftlineflag[i + 2] &&
            f1.leftlineflag[i] &&
            f1.leftlineflag[i - 2] &&
            f1.leftlineflag[i - 4] 

            // abs(f1.leftline[i - 6] - f1.leftline[i]) >= 2
            )
        {
            f2.L_loop_down_row = i;
            break;
        }
    }

    for (int i = f2.L_loop_down_row; i > f2.toppoint; i--)
    {
        if (
            f1.leftline[i] > 20 &&

            f1.leftline[i] > f1.leftline[i - 4] &&
            f1.leftline[i] > f1.leftline[i + 4] &&
            f1.leftline[i] > f1.leftline[i + 6] &&

            f1.leftlineflag[i - 4] &&
            f1.leftlineflag[i] &&
            f1.leftlineflag[i + 4] &&
            f1.leftlineflag[i + 6])
        {
            f2.L_loop_circle_row = i;
            f2.L_loop_circle_col = f1.leftline[i];
            break;
        }
    }

    for (int i = f2.L_loop_circle_row; i > f1.tag_toppoint; i--)
    {
        if (
            f1.tag_leftlineflag[i + 4] &&
            f1.tag_leftlineflag[i + 2] &&
            f1.tag_leftline[i + 2] - f1.tag_leftline[i] > 10)
        {
            f2.L_loop_up_row = i;
            break;
        }
    }

    for (int i = f2.L_loop_up_row; i > f1.tag_toppoint; i--)
    {
        if (
            // f1.tag_leftlineflag[i - 4] &&
            f1.tag_leftlineflag[i - 2] &&
            f1.tag_leftlineflag[i] &&

            abs(f1.tag_leftline[i] - f1.tag_leftline[i - 2]) <= 4 &&
            // abs(f1.tag_leftline[i - 2] - f1.tag_leftline[i - 4]) <= 4 &&

            f1.tag_leftline[i] - f1.tag_leftline[i + 2] > 18)

        {
            f2.L_loop_top_row = i;
            break;
        }
    }

    if ((f1.rightstartrow >= 118 ||

         (f1.rightstartrow > 90 &&
          f1.rightstartrow < 118 &&
          f1.rightline[f1.rightstartrow] > 155)) &&

        // f2.L_loop_down_row < 80 &&
        f1.leftstartrow<80&&
        f2.L_loop_top_row < 40 &&

        is_L_R_nonlose(f2.L_loop_down_row - 1,
                       f2.L_loop_circle_row,
                       0, 100) &&

        is_L_R_nonlose(f2.L_loop_top_row,
                       f1.rightstartrow,
                       1, 100) &&

        is_straight_line(f2.L_loop_top_row,
                         f1.tag_R_start_row,
                         f1.tag_rightline, 5))
    {
        f2.L_loop_flag = 1;
        f2.L_loop_frame++;
    }
    else
    {
        f2.L_loop_flag = 0;
        f2.L_loop_frame = 0;
    }
}

void R_loop_judge()
{
    f2.R_loop_down_row = 0;
    f2.R_loop_circle_row = 0;
    f2.R_loop_up_row = 0;
    f2.R_loop_top_row = 0;

    find_L_edge_from_inner();
    find_R_egde_from_L();
    find_topstart_row();
    find_L_tag_toppoint();

    for (int i = 100; i > f2.toppoint; i--)
    {
        if (
            !f1.rightlineflag[i + 15] &&
            !f1.rightlineflag[i + 10] &&
            !f1.rightlineflag[i + 4] &&
            !f1.rightlineflag[i + 2] &&
            f1.rightlineflag[i] &&
            f1.rightlineflag[i - 2] &&
            f1.rightlineflag[i - 4] 

            // abs(f1.rightline[i - 6] - f1.rightline[i]) >= 2
            )
        {
            f2.R_loop_down_row = i;
            break;
        }
    }

    for (int i = f2.R_loop_down_row; i > f2.toppoint; i--)
    {
        if (
            f1.rightline[i] < 140 &&

            f1.rightline[i] < f1.rightline[i - 4] &&
            f1.rightline[i] < f1.rightline[i + 4] &&
            f1.rightline[i] < f1.rightline[i + 6] &&

            f1.rightlineflag[i - 4] &&
            f1.rightlineflag[i] &&
            f1.rightlineflag[i + 4] &&
            f1.rightlineflag[i + 6])
        {
            f2.R_loop_circle_row = i;
            f2.R_loop_circle_col = f1.rightline[i];
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
            // f1.tag_rightlineflag[i - 4] &&
            f1.tag_rightlineflag[i - 2] &&
            f1.tag_rightlineflag[i] &&

            abs(f1.tag_rightline[i] - f1.tag_rightline[i - 2]) <= 4 &&
            // abs(f1.tag_rightline[i - 2] - f1.tag_rightline[i - 4]) <= 4 &&

            f1.tag_rightline[i + 2] - f1.tag_rightline[i] > 18)
        {
            f2.R_loop_top_row = i;
            break;
        }
    }

    if ((f1.leftstartrow >= 118 ||
         (f1.leftstartrow > 90 &&
          f1.leftstartrow < 118 &&
          f1.leftline[f1.leftstartrow] <= 3)) &&

        // f2.R_loop_down_row < 90 &&
        f1.rightstartrow<80&&
        f2.R_loop_top_row < 40 &&

        is_L_R_nonlose(f2.R_loop_down_row - 1,
                       f2.R_loop_circle_row,
                       1, 100) &&

        is_L_R_nonlose(f2.R_loop_top_row,
                       f1.leftstartrow,
                       0, 100) &&

        is_straight_line(f2.R_loop_top_row,
                         f1.tag_L_start_row,
                         f1.tag_leftline, 5))
    {
        f2.R_loop_flag = 1;
        f2.R_loop_frame++;
    }
    else
    {
        f2.R_loop_flag = 0;
        f2.R_loop_frame = 0;
    }
}

////////////////////////////////////////
// patch--------------------------------
////////////////////////////////////////
void L_loop_patch()
{

    if (f2.L_loop_sm_state == 1 || f2.L_loop_sm_state == 3)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
        }

        L_mid_endrow_proc();

        find_L_loop_in_infec();
        if (f2.L_loop_up_infec_row > 34 &&
            chassis.loop_distance >= 12)
        {
            f2.L_loop_sm_state = 2;
            chassis.loop_distance = 0;
        }

        // if (f2.L_loop_sm_state == 1)
        //     if (f1.leftstartrow < 77 && chassis.loop_distance >= 5)
        //     {
        //         f2.L_loop_sm_state = 3;
        //         chassis.loop_distance = 0;
        //     }

        // if (f2.L_loop_sm_state == 3)
        //     if (f1.leftstartrow > 105 && chassis.loop_distance >= 10)
        //     {
        //         f2.L_loop_sm_state = 2;
        //         chassis.loop_distance = 0;
        //     }

        //   if (f2.L_loop_sm_state == 1)
        //     if (f1.leftstartrow < 83)
        //     {
        //         f2.L_loop_sm_state = 3;
        //         chassis.loop_distance = 0;
        //     }

        // if (f2.L_loop_sm_state == 3)
        //     if (f1.leftstartrow > 105)
        //     {
        //         f2.L_loop_sm_state = 2;
        //         chassis.loop_distance = 0;
        //     }
    }

    if (f2.L_loop_sm_state == 2 || f2.L_loop_sm_state == 4)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.leftlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1] - 3;
            else if (i >= 108 && f1.leftlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1] - 1;
            else
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);

            if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
                f1.midline[i] = f1.midline[i + 1];

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (f2.L_loop_sm_state == 2)
            if (f1.rightstartrow < 75)
                f2.L_loop_sm_state = 4;

        if (f2.L_loop_sm_state == 4)
            if (f1.rightstartrow > 110)
                f2.L_loop_sm_state = 5;
    }

    // if (f2.L_loop_sm_state == 5)
    // {
    //     int start_offset = 5;
    //     start_offset = (int)(f2.toppoint * 0.22);
    //     __Limit_AB(start_offset, 0, 60);

    //     for (int i = 118; i > 110; i--)
    //     {
    //         f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i])-6;
    //     }

    //     for (int i = 115; i > 35; i--)
    //     {
    //         f1.midline[i] = f1.midline[i+1] - (f1.rightline[i+1]-f1.rightline[i]);

    //         if (f1.midline[i] >= f1.midline[i + 1])
    //             f1.midline[i] = f1.midline[i + 1]-1;

    //         L_MID_BREAK
    //     }

    //     L_mid_endrow_proc();

    //     if (fabs(inoutdev.gyro_z_angle) >= 335)
    //     {
    //         chassis.loop_distance = 0;
    //         f2.L_loop_sm_state = 7;
    //     }
    // }

    if (f2.L_loop_sm_state == 5)
    {
        for (int i = 110; i > 35; i--)
        {
            f1.midline[i] = f1.rightline[i] - (int16)(loop_half_width[i]) - 6;

            if (i < 90 && f1.midline[i] > f1.midline[i + 1])
                f1.midline[i] = f1.midline[i + 1] - 1;

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 250)
            f2.L_loop_sm_state = 6;
    }

    if (f2.L_loop_sm_state == 6)
    {
        int start_offset = 5;
        start_offset = (int)(f2.toppoint * 0.22);
        __Limit_AB(start_offset, 0, 60);

        for (int i = 118; i > 35; i--)
        {
            if (i == 118 || f1.leftlineflag[i])
                f1.midline[i] = f1.leftline[i] + half_width[i] - start_offset;
            else
                f1.midline[i] = f1.midline[i + 1] - 1;

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 335)
        {
            chassis.loop_distance = 0;
            f2.L_loop_sm_state = 7;
        }
    }

    // if (f2.L_loop_sm_state == 7 || f2.L_loop_sm_state == 8)
    // {
    //     for (int i = 110; i > f2.toppoint; i--)
    //     {
    //         f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);

    //         L_MID_BREAK
    //     }

    //     L_mid_endrow_proc();

    //     if (f2.L_loop_sm_state == 7)
    //         if (
    //             f1.leftstartrow < 80 &&
    //             chassis.loop_distance > 5)
    //         {
    //             f2.L_loop_sm_state = 8;
    //             chassis.loop_distance = 0;
    //         }

    //     if (f2.L_loop_sm_state == 8)
    //         if ((
    //                 f1.leftstartrow > 108 &&
    //                 chassis.loop_distance > 5) ||

    //             chassis.loop_distance > 100)
    //         {
    //             inoutdev.beep(2);
    //             f2.L_loop_sm_state = 0;
    //             enable_all_elements();
    //             inoutdev.gyro_y_angle = 0;
    //             inoutdev.cross_z_angle = 0;
    //         }
    // }
    if (f2.L_loop_sm_state == 7)
    {
        for (int i = 110; i > f2.toppoint; i--)
        {
            f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if ((f1.leftstartrow > 108 &&
             chassis.loop_distance > 55) ||

            chassis.loop_distance > 200)
        {
            inoutdev.beep(2);
            f2.L_loop_sm_state = 0;
            enable_all_elements();
            inoutdev.gyro_y_angle = 0;
            inoutdev.cross_z_angle = 0;
        }
    }
}

void R_loop_patch()
{
    if (f2.R_loop_sm_state == 1 || f2.R_loop_sm_state == 3)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.leftlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        find_R_loop_in_infec();
        if (f2.R_loop_up_infec_row > 38 &&
            chassis.loop_distance >= 12)
        {
            f2.R_loop_sm_state = 2;
            chassis.loop_distance = 0;
        }

        // if (f2.R_loop_sm_state == 1)
        //     if (f1.rightstartrow < 80 && chassis.loop_distance >= 5)
        //     {
        //         f2.R_loop_sm_state = 3;
        //         chassis.loop_distance = 0;
        //     }

        // if (f2.R_loop_sm_state == 3)
        //     if (f1.rightstartrow > 105 && chassis.loop_distance >= 10)
        //     {
        //         f2.R_loop_sm_state = 2;
        //         chassis.loop_distance = 0;
        //     }

        // if (f2.R_loop_sm_state == 1)
        //     if (f1.rightstartrow < 83)
        //     {
        //         f2.R_loop_sm_state = 3;
        //         chassis.loop_distance = 0;
        //     }

        // if (f2.R_loop_sm_state == 3)
        //     if (f1.rightstartrow > 95)
        //     {
        //         f2.R_loop_sm_state = 2;
        //         chassis.loop_distance = 0;
        //     }
    }

    if (f2.R_loop_sm_state == 2 || f2.R_loop_sm_state == 4)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1] + 2;
            else if (i >= 108 && f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1] + 1;
            else
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i])-3;

            if (abs(f1.midline[i] - f1.midline[i + 1]) > 40)
                f1.midline[i] = f1.midline[i + 1];

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (f2.R_loop_sm_state == 2)
            if (is_L_R_lose(90, 118, 0, 90))
                f2.R_loop_sm_state = 4;

        if (f2.R_loop_sm_state == 4)
            if (is_L_R_nonlose(80, 110, 0, 80))
                f2.R_loop_sm_state = 5;
    }

    if (f2.R_loop_sm_state == 5)
    {
        for (int i = 110; i > 35; i--)
        {
            f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]) + 11;

            if (i < 90 && f1.midline[i] < f1.midline[i + 1])
                f1.midline[i] = f1.midline[i + 1] + 1;

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 255)
            f2.R_loop_sm_state = 6;
    }

    if (f2.R_loop_sm_state == 6)
    {
        int start_offset = 5;
        start_offset = (int)(f2.toppoint * 0.18);
        __Limit_AB(start_offset, 0, 60);

        for (int i = 118; i > 35; i--)
        {
            if (i == 118 || f1.rightlineflag[i])
                f1.midline[i] = f1.rightline[i] - half_width[i] + start_offset;
            else
                f1.midline[i] = f1.midline[i + 1] + 1;

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (fabs(inoutdev.gyro_z_angle) >= 335)
        {
            f2.R_loop_sm_state = 7;
            chassis.loop_distance = 0;
        }
    }

    if (f2.R_loop_sm_state == 7)
    {
        for (int i = 110; i > f2.toppoint; i--)
        {
            f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);

            R_MID_BREAK
        }

        R_mid_endrow_proc();

        if (
            // (f1.rightstartrow > 108 &&
            //  chassis.loop_distance > 30) ||

            chassis.loop_distance > 75
            )
        {
            inoutdev.beep(2);
            f2.R_loop_sm_state = 0;
            enable_all_elements();
            inoutdev.gyro_y_angle = 0;
            inoutdev.cross_z_angle = 0;
        }
    }
}

////////////////////////////////////////
// tools--------------------------------
////////////////////////////////////////
void find_L_loop_in_infec()
{
    find_R_edge_from_inner();
    find_L_egde_from_R();

    f2.L_loop_Redge_endrow = 0;
    for (int i = 100; i > 10; i--)
    {
        if (abs(f1.tag_rightline[i] - f1.tag_rightline[i - 1]) > 6)
        {
            f2.L_loop_Redge_endrow = i;
            break;
        }
    }

    f2.L_loop_up_infec_row = 0;
    for (int i = 100; i > f2.L_loop_Redge_endrow; i--)
    {
        if (
            f1.tag_leftline[i] > 20 &&

            f1.tag_leftlineflag[i - 9] == 1 &&
            f1.tag_leftlineflag[i - 5] == 1 &&
            f1.tag_leftlineflag[i - 3] == 1 &&
            f1.tag_leftlineflag[i] == 1 &&

            abs(f1.tag_leftline[i] - f1.tag_leftline[i - 3]) < 4 &&
            abs(f1.tag_leftline[i - 6] - f1.tag_leftline[i - 9]) < 4 &&

            is_straight_line(i, i - 9, f1.tag_leftline, 4) &&
            f1.tag_leftline[i] - f1.tag_leftline[i + 3] > 20 &&

            (f1.tag_leftlineflag[i + 2] == 0 ||
             f1.tag_leftlineflag[i + 4] == 0))
        {
            f2.L_loop_up_infec_row = i;
            break;
        }
    }
}

void find_R_loop_in_infec()
{

    find_L_edge_from_inner();
    find_R_egde_from_L();
    f2.R_loop_Ledge_endrow = 0;
    f2.R_loop_up_infec_row = 0;
    for (int i = 100; i > 10; i--)
    {
        if (abs(f1.tag_leftline[i] - f1.tag_leftline[i - 1]) > 6)
        {
            f2.R_loop_Ledge_endrow = i;
            break;
        }
    }
    for (int i = 100; i > f2.R_loop_Ledge_endrow; i--)
    {
        if (
            f1.tag_rightline[i] < 140 &&

            f1.tag_rightlineflag[i - 9] == 1 &&
            f1.tag_rightlineflag[i - 5] == 1 &&
            f1.tag_rightlineflag[i - 3] == 1 &&
            f1.tag_rightlineflag[i] == 1 &&

            abs(f1.tag_rightline[i] - f1.tag_rightline[i - 3]) < 4 &&
            abs(f1.tag_rightline[i - 6] - f1.tag_rightline[i - 9]) < 4 &&

            f1.tag_rightline[i + 3] - f1.tag_rightline[i] > 20 &&

            (f1.tag_rightlineflag[i + 2] == 0 ||
             f1.tag_rightlineflag[i + 4] == 0))
        {
            f2.R_loop_up_infec_row = i;
            break;
        }
    }
}

void L_loop_patch_robust()
{

    if (f2.L_loop_sm_state == 1 || f2.L_loop_sm_state == 3)
    {
        for (int i = 117; i > f2.toppoint; i--)
        {
            if (f1.rightlineflag[i] == 0)
                f1.midline[i] = f1.midline[i + 1];
            else
                f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
        }

        L_mid_endrow_proc();

        if (f2.L_loop_sm_state == 1)
            if (f1.leftstartrow < 77 && chassis.loop_distance >= 5)
            {
                f2.L_loop_sm_state = 3;
                chassis.loop_distance = 0;
            }

        if (f2.L_loop_sm_state == 3)
            if (f1.leftstartrow > 105 && chassis.loop_distance >= 5)
            {
                f2.L_loop_sm_state = 2;
                chassis.loop_distance = 0;
            }
    }

    if (f2.L_loop_sm_state == 2 || f2.L_loop_sm_state == 4)
    {
        for (int i = 110; i > 35; i--)
        {
            if (i < 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 3;
            }
            else if (i >= 108 && f1.leftlineflag[i] == 0)
            {
                f1.midline[i] = f1.midline[i + 1] - 1;
            }
            else
            {
                f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);
            }

            if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
                f1.midline[i] = f1.midline[i + 1];

            L_MID_BREAK
        }

        L_mid_endrow_proc();

        if (f2.L_loop_sm_state == 2)
            if (f1.rightstartrow < 75)
                f2.L_loop_sm_state = 4;

        if (f2.L_loop_sm_state == 4)
            if (f1.rightstartrow > 110)
                f2.L_loop_sm_state = 5;
    }

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

    if (f2.L_loop_sm_state == 6)
    {
        int start_offset = 5;
        start_offset = (int)(f2.toppoint * 0.22);
        __Limit_AB(start_offset, 0, 60);

        for (int i = 118; i > 35; i--)
        {
            if (i == 118 || f1.leftlineflag[i])
            {
                f1.midline[i] = f1.leftline[i] + half_width[i] - start_offset;
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

    if (f2.L_loop_sm_state == 7)
    {
        for (int i = 110; i > f2.toppoint; i--)
        {
            f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);

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
