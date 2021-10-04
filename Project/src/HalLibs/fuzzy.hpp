#ifndef __FUZZY__
#define __FUZZY__
#include "common_macro.h"
#include "rtthread.h"
using namespace rtthread;

#define FMAX 100 //语言值得满赋值

class steer_fuzzy
{
public:
    float U;
    // float UU = 0;
    // float U_d_max = 60;
    /*偏差,偏差微分以及输出值的精确量*/
    float PF[2] = {0}, DF[2] = {0}, UF[4] = {0};
    /*偏差,偏差微分以及输出值的隶属度*/ //unsigned int
    int Pn, Dn, Un[4];
    float temp1, temp2;
    float a1 = 0, a2 = 0, a3 = 0, a4 = 0;
    int PFF[9] = {-70, -50, -25, -10, 0, 10, 25, 50, 70};
    //int DFF[7] = {-18, -12, -6, 0, 6, 12, 18};
    int DFF[7] = {-15, -10, -5, 0, 5, 10, 15};
    // float UFF[9] = {0, 2, 4, 8, 12, 16, 20, 24, 28};
    float UFF[9] = {0, 10, 24, 40, 60, 80, 100, 125, 145};

    int rule[9][7] = {
        //ec大于-90小于-45为-2  ec大于-45小于0为-1
        //ec大于0小于45为0 ec大于45小于90为1
        //-3 -2 -1 0 1 2 3   ec/e
        // {6, 7, 6, 7, 6, 5, 5}, //  -4
        // {5, 8, 6, 6, 5, 4, 4}, //  -3
        // {6, 6, 5, 5, 4, 3, 2}, //  -2
        // {5, 4, 3, 3, 2, 1, 1}, //  -1
        // {3, 2, 1, 0, 1, 2, 3}, //  -0
        // {1, 1, 2, 3, 3, 4, 5}, //   1
        // {2, 3, 4, 5, 5, 6, 6}, //   2
        // {4, 4, 5, 6, 6, 8, 5}, //   3
        // {5, 5, 6, 7, 6, 7, 6}, //   4
        //-3 -2 -1 0 1 2 3   ec/e
        {5, 6, 6, 8, 6, 5, 5}, //  -4
        {5, 8, 6, 7, 5, 4, 4}, //  -3
        {6, 6, 5, 5, 4, 3, 2}, //  -2
        {5, 4, 3, 3, 2, 1, 1}, //  -1
        {3, 2, 1, 0, 1, 2, 3}, //  -0
        {1, 1, 2, 3, 3, 4, 5}, //   1
        {2, 3, 4, 5, 5, 6, 6}, //   2
        {4, 4, 5, 7, 6, 8, 5}, //   3
        {5, 5, 6, 8, 6, 6, 5}, //   4
    };

    //     {6, 7, 6, 5, 5, 4, 4}, //  -4
    //     {7, 6, 5, 4, 4, 3, 3}, //  -3
    //     {6, 5, 5, 4, 3, 2, 2}, //  -2
    //     {5, 4, 3, 2, 2, 1, 1}, //  -1
    //     {3, 2, 1, 0, 1, 2, 3}, //  -0
    //     {1, 1, 2, 2, 3, 4, 5}, //   1
    //     {2, 2, 3, 4, 5, 5, 6}, //   2
    //     {3, 3, 4, 4, 5, 6, 7}, //   3
    //     {4, 4, 5, 5, 6, 7, 6}, //   4

    float fuzzy(int P, int D)
    {
        /*隶属度的确定*/
        /*根据PD的指定语言值获得有效隶属度*/
        if (P > PFF[0] && P < PFF[8])
        {
            if (P <= PFF[1])
            {
                Pn = -2;
                PF[0] = (float)(FMAX * ((float)(PFF[1] - P) / (PFF[1] - PFF[0]))); //求p值占该论域的比例即为在该论域的概率
            }
            else if (P <= PFF[2])
            {
                Pn = -1;
                PF[0] = (float)(FMAX * ((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
            }
            else if (P <= PFF[3])
            {
                Pn = 0;
                PF[0] = (float)(FMAX * ((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
            }
            else if (P <= PFF[4])
            {
                Pn = 1;
                PF[0] = (float)(FMAX * ((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
            }
            else if (P <= PFF[5])
            {
                Pn = 2;
                PF[0] = (float)(FMAX * ((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
            }
            else if (P <= PFF[6])
            {
                Pn = 3;
                PF[0] = (float)(FMAX * ((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
            }
            else if (P <= PFF[7])
            {
                Pn = 4;
                PF[0] = (float)(FMAX * ((float)(PFF[7] - P) / (PFF[7] - PFF[6])));
            }
            else if (P <= PFF[8])
            {
                Pn = 5;
                PF[0] = (float)(FMAX * ((float)(PFF[8] - P) / (PFF[8] - PFF[7])));
            }
        }
        else if (P <= PFF[0])
        {
            Pn = -2;
            PF[0] = FMAX;
        }
        else if (P >= PFF[8])
        {
            Pn = 5;
            PF[0] = 0;
        }
        PF[1] = FMAX - PF[0]; //求p值不在所在论域的概率

        if (D > DFF[0] && D < DFF[6]) //-10~10
        {
            if (D <= DFF[1]) //-10~-5
            {
                Dn = -2;
                DF[0] = (float)(FMAX * ((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
            }
            else if (D <= DFF[2]) //-5~-0
            {
                Dn = -1;
                DF[0] = (float)(FMAX * ((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
            }
            else if (D <= DFF[3]) //0~5
            {
                Dn = 0;
                DF[0] = (float)(FMAX * ((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
            }
            else if (D <= DFF[4]) //5~10
            {
                Dn = 1;
                DF[0] = (float)(FMAX * ((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
            }
            else if (D <= DFF[5]) //5~10
            {
                Dn = 2;
                DF[0] = (float)(FMAX * ((float)(DFF[5] - D) / (DFF[5] - DFF[4])));
            }
            else if (D <= DFF[6]) //5~10
            {
                Dn = 3;
                DF[0] = (float)(FMAX * ((float)(DFF[6] - D) / (DFF[6] - DFF[5])));
            }
        }
        else if (D <= DFF[0])
        {
            Dn = -2;
            DF[0] = FMAX;
        }
        else if (D >= DFF[6])
        {
            Dn = 3;
            DF[0] = 0;
        }
        DF[1] = FMAX - DF[0];
        /*使用误差范围优化后的规则表rule[7][7]*/
        /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
        /*一般都是四个规则有效*/
        Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
        Un[1] = rule[Pn + 3][Dn - 1 + 3];
        Un[2] = rule[Pn - 1 + 3][Dn + 3];
        Un[3] = rule[Pn + 3][Dn + 3];
        if (PF[0] <= DF[0])
            UF[0] = PF[0]; //Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
        else
            UF[0] = DF[0];

        if (PF[1] <= DF[0])
            UF[1] = PF[1]; //Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
        else
            UF[1] = DF[0];

        if (PF[0] <= DF[1])
            UF[2] = PF[0]; //Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
        else
            UF[2] = DF[1];

        if (PF[1] <= DF[1]) //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
            UF[3] = PF[1];
        else
            UF[3] = DF[1];
        /*同隶属函数输出语言值求大*/

        if (Un[0] == Un[1])
        {
            if (UF[0] > UF[1]) //UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
                UF[1] = 0;     //概率小的清零
            else
                UF[0] = 0;
        }
        if (Un[0] == Un[2])
        {
            if (UF[0] > UF[2])
                UF[2] = 0;
            else
                UF[0] = 0;
        }
        if (Un[0] == Un[3])
        {
            if (UF[0] > UF[3])
                UF[3] = 0;
            else
                UF[0] = 0;
        }
        if (Un[1] == Un[2])
        {
            if (UF[1] > UF[2])
                UF[2] = 0;
            else
                UF[1] = 0;
        }
        if (Un[1] == Un[3])
        {
            if (UF[1] > UF[3])
                UF[3] = 0;
            else
                UF[1] = 0;
        }
        if (Un[2] == Un[3])
        {
            if (UF[2] > UF[3])
                UF[3] = 0;
            else
                UF[2] = 0;
        }

        a1 = UF[0] * UFF[Un[0]];
        a2 = UF[1] * UFF[Un[1]];
        a3 = UF[2] * UFF[Un[2]];
        a4 = UF[3] * UFF[Un[3]];

        temp1 = a1 + a2 + a3 + a4;
        temp2 = UF[0] + UF[1] + UF[2] + UF[3];
        U = (temp1 / temp2); //加权平均(int)

        return U;
    }
};

class motor_fuzzy
{
public:
    int Motor_U;
    /*偏差,偏差微分以及输出值的精确量*/
    uint32_t Motor_PF[2] = {0};
    uint32_t Motor_DF[2] = {0};
    uint32_t Motor_UF[4] = {0};
    /*偏差,偏差微分以及输出值的隶属度*/
    int Motor_Pn, Motor_Dn, Motor_Un[4];
    float Motor_temp1, Motor_temp2;
    float Motor_a1 = 0;
    float Motor_a2 = 0;
    float Motor_a3 = 0;
    float Motor_a4 = 0;

    int Motor_PFF[7] = {-100, -60, -30, 0, 30, 60, 100};
    int Motor_DFF[5] = {-30, -10, 0, 10, 30};
    int Motor_UFF[7] = {0, 10, 15, 20, 25, 30, 40};

    int Motor_rule[7][5] = {
        //  -2  -1   0   1    ec    e
        {6, 6, 5, 3, 2}, //  -2
        {5, 4, 4, 2, 1}, //  -1
        {3, 2, 2, 1, 0}, //  0
        {2, 2, 1, 0, 0}, //   1
        {0, 1, 2, 2, 3}, //   2
        {1, 2, 4, 4, 5}, //   3
        {2, 3, 5, 6, 6}  //
    };

    int fuzzy(int P, int D)
    {
        /*隶属度的确定*/
        /*根据PD的指定语言值获得有效隶属度*/
        if (P > Motor_PFF[0] && P < Motor_PFF[6])
        {
            if (P <= Motor_PFF[1])
            {
                Motor_Pn = -2;
                Motor_PF[0] = (int)((float)(Motor_PFF[1] - P) / (Motor_PFF[1] - Motor_PFF[0])); //求p值占该论域的比例即为在该论域的概率
            }
            else if (P <= Motor_PFF[2])
            {
                Motor_Pn = -1;
                Motor_PF[0] = (int)((float)(Motor_PFF[2] - P) / (Motor_PFF[2] - Motor_PFF[1]));
            }
            else if (P <= Motor_PFF[3])
            {
                Motor_Pn = 0;
                Motor_PF[0] = (int)((float)(Motor_PFF[3] - P) / (Motor_PFF[3] - Motor_PFF[2]));
            }
            else if (P <= Motor_PFF[4])
            {
                Motor_Pn = 1;
                Motor_PF[0] = (int)((float)(Motor_PFF[4] - P) / (Motor_PFF[4] - Motor_PFF[3]));
            }
            else if (P <= Motor_PFF[5])
            {
                Motor_Pn = 2;
                Motor_PF[0] = (int)((float)(Motor_PFF[5] - P) / (Motor_PFF[5] - Motor_PFF[4]));
            }
            else if (P <= Motor_PFF[6])
            {
                Motor_Pn = 3;
                Motor_PF[0] = (int)((float)(Motor_PFF[6] - P) / (Motor_PFF[6] - Motor_PFF[5]));
            }
        }
        else if (P <= Motor_PFF[0])
        {
            Motor_Pn = -2;
            Motor_PF[0] = 1;
        }
        else if (P >= Motor_PFF[6])
        {
            Motor_Pn = 3;
            Motor_PF[0] = 0;
        }
        Motor_PF[1] = 1 - Motor_PF[0]; //求p值不在所在论域的概率

        /***********计算DDDDDDDDD*****************/
        if (D > Motor_DFF[0] && D < Motor_DFF[4]) //-10~10
        {
            if (D <= Motor_DFF[1]) //-10~-5
            {
                Motor_Dn = -2;
                Motor_DF[0] = (int)((float)(Motor_DFF[1] - D) / (Motor_DFF[1] - Motor_DFF[0]));
            }
            else if (D <= Motor_DFF[2]) //-5~-0
            {
                Motor_Dn = -1;
                Motor_DF[0] = (int)((float)(Motor_DFF[2] - D) / (Motor_DFF[2] - Motor_DFF[1]));
            }
            else if (D <= Motor_DFF[3]) //0~5
            {
                Motor_Dn = 0;
                Motor_DF[0] = (int)((float)(Motor_DFF[3] - D) / (Motor_DFF[3] - Motor_DFF[2]));
            }
            else if (D <= Motor_DFF[4]) //5~10
            {
                Motor_Dn = 1;
                Motor_DF[0] = (int)((float)(Motor_DFF[4] - D) / (Motor_DFF[4] - Motor_DFF[3]));
            }
        }
        else if (D <= Motor_DFF[0])
        {
            Motor_Dn = -2;
            Motor_DF[0] = 1;
        }
        else if (D >= Motor_DFF[4])
        {
            Motor_Dn = 1;
            Motor_DF[0] = 0;
        }
        Motor_DF[1] = 1 - Motor_DF[0];
        /*使用误差范围优化后的规则表rule[7][7]*/
        /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
        /*一般都是四个规则有效*/
        Motor_Un[0] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn - 1 + 3];
        Motor_Un[1] = Motor_rule[Motor_Pn + 3][Motor_Dn - 1 + 3];
        Motor_Un[2] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn + 3];
        Motor_Un[3] = Motor_rule[Motor_Pn + 3][Motor_Dn + 3];
        if (Motor_PF[0] <= Motor_DF[0])
            Motor_UF[0] = Motor_PF[0]; //Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
        else
            Motor_UF[0] = Motor_DF[0];

        if (Motor_PF[1] <= Motor_DF[0])
            Motor_UF[1] = Motor_PF[1]; //Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
        else
            Motor_UF[1] = Motor_DF[0];

        if (Motor_PF[0] <= Motor_DF[1])
            Motor_UF[2] = Motor_PF[0]; //Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
        else
            Motor_UF[2] = Motor_DF[1];

        if (Motor_PF[1] <= Motor_DF[1]) //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
            Motor_UF[3] = Motor_PF[1];
        else
            Motor_UF[3] = Motor_DF[1];
        /*同隶属函数输出语言值求大*/

        if (Motor_Un[0] == Motor_Un[1])
        {
            if (Motor_UF[0] > Motor_UF[1]) //UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
                Motor_UF[1] = 0;           //概率小的清零
            else
                Motor_UF[0] = 0;
        }
        if (Motor_Un[0] == Motor_Un[2])
        {
            if (Motor_UF[0] > Motor_UF[2])
                Motor_UF[2] = 0;
            else
                Motor_UF[0] = 0;
        }
        if (Motor_Un[0] == Motor_Un[3])
        {
            if (Motor_UF[0] > Motor_UF[3])
                Motor_UF[3] = 0;
            else
                Motor_UF[0] = 0;
        }
        if (Motor_Un[1] == Motor_Un[2])
        {
            if (Motor_UF[1] > Motor_UF[2])
                Motor_UF[2] = 0;
            else
                Motor_UF[1] = 0;
        }
        if (Motor_Un[1] == Motor_Un[3])
        {
            if (Motor_UF[1] > Motor_UF[3])
                Motor_UF[3] = 0;
            else
                Motor_UF[1] = 0;
        }
        if (Motor_Un[2] == Motor_Un[3])
        {
            if (Motor_UF[2] > Motor_UF[3])
                Motor_UF[3] = 0;
            else
                Motor_UF[2] = 0;
        }

        Motor_a1 = Motor_UF[0] * Motor_UFF[Motor_Un[0]];
        Motor_a2 = Motor_UF[1] * Motor_UFF[Motor_Un[1]];
        Motor_a3 = Motor_UF[2] * Motor_UFF[Motor_Un[2]];
        Motor_a4 = Motor_UF[3] * Motor_UFF[Motor_Un[3]];

        Motor_temp1 = Motor_a1 + Motor_a2 + Motor_a3 + Motor_a4;
        Motor_temp2 = Motor_UF[0] + Motor_UF[1] + Motor_UF[2] + Motor_UF[3];
        Motor_U = (int)(Motor_temp1 / Motor_temp2); //加权平均
        return Motor_U;
    }
};

#endif