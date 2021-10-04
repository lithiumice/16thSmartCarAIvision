#include "cstdio"
#include "cmath"
#include <iostream>
#include "f_elems.h"
#include "img_proc.h"
#include "main.h"

int read_txt_120(const char *file)
{
    FILE *fp = fopen(file, "rb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
        return 0;
    }
    int temp = 0;
    int count = 0;
    while (fscanf(fp, "%3d", &temp))
    {
        int row = (int)(floor(count / 160));
        bin_120_160_image[row][count % 160] = temp;
        if (temp == -1)
            break;
        temp = -1;
        count++;
    }
    fclose(fp);
    return 1;
}

int read_txt_240(const char *file)
{
    FILE *fp = fopen(file, "rb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
        return 0;
    }
    int temp = 0;
    int count = 0;
    while (fscanf(fp, "%3d", &temp))
    {
        int row = (int)(floor(count / 320));
        gray_240_320_image[row][count % 320] = temp;
        if (temp == -1)
            break;
        temp = -1;
        count++;
    }
    fclose(fp);
    return 1;
}

void save_txt_120(const char *file)
{
    FILE *fp = fopen(file, "wb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
    }
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 160; j++)
        {
            fprintf(fp, "%3d", IMG_120_160[i][j]);
        }
        fprintf(fp, "%s", "\r\n");
    }
    printf("save txt img %s done\n", file);
    fclose(fp);
}

void save_txt_240(const char *file)
{
    FILE *fp = fopen(file, "wb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
    }
    for (int i = 0; i < 240; i++)
    {
        for (int j = 0; j < 320; j++)
        {
            fprintf(fp, "%3d", IMG_240_320[i][j]);
        }
        fprintf(fp, "%s", "\r\n");
    }
    printf("save txt img %s done\n", file);
    fclose(fp);
}

void draw_midline()
{
    for (int i = 0; i < 120; i++)
    {
        IMG_120_160[i][f1.midline[i]] = 0;
        IMG_120_160[i][f1.leftline[i]] = 0;
        IMG_120_160[i][f1.rightline[i]] = 0;
    }

    for (int i = 0; i < 240; i++)
    {
        IMG_240_320[i][2 * f1.midline[i / 2]] = 0;
        IMG_240_320[i][2 * f1.leftline[i / 2]] = 0;
        IMG_240_320[i][2 * f1.rightline[i / 2]] = 0;
    }
}

////////////////////

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
            //TODO
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
