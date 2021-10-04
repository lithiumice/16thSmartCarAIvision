#include "headfile.h"
#include "f_elems.h"
#include "img_proc.h"
#include "main.h"

//////////////////////////////////////////////////////////
void push_and_pull(float *buff, int len, float newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

void push_int_pull(int *buff, int len, int newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

int clip(int x, int low, int up)
{
  return x > up ? up : x < low ? low
                               : x;
}

float fclip(float x, float low, float up)
{
  return x > up ? up : x < low ? low
                               : x;
}

float rt_millisecond_from_tick(rt_tick_t ticks)
{
  return (float)ticks / RT_TICK_PER_SECOND * 1000;
}

void laser_on()
{
  inoutdev.laser_on();
}

void laser_off()
{
  inoutdev.laser_off();
}

void LPF_1_db(float hz, float time, float in, float *out)
{
  *out += (1 / (1 + 1 / (hz * 6.28f * time))) * (in - *out);
}

int str2bool(char *str)
{
  if (strcmp(str, "0") == 0)
    return RT_FALSE;
  else
    return RT_TRUE;
}

void reboot(void)
{
  NVIC_SystemReset();
}
MSH_CMD_EXPORT(reboot, reset system);

void upload_zf_bin()
{
  uart_putchar(USART_1, 0x00);
  uart_putchar(USART_1, 0xff);
  uart_putchar(USART_1, 0x01);
  uart_putchar(USART_1, 0x01);
  uart_putbuff(USART_1, bin_240_320_image[0], IMG_W * IMG_H);
}
MSH_CMD_EXPORT(upload_zf_bin, upload_zf_bin);

void upload_zf_gray()
{
  uart_putchar(USART_1, 0x00);
  uart_putchar(USART_1, 0xff);
  uart_putchar(USART_1, 0x01);
  uart_putchar(USART_1, 0x01);
  uart_putbuff(USART_1, mt9v03x_csi_image[0], IMG_W * IMG_H);
}
MSH_CMD_EXPORT(upload_zf_gray, upload_zf_gray);

void get_half_width()
{
  for (int i = 0; i < 120; i++)
  {
    int w = f1.rightline[i] - f1.leftline[i];
    rt_kprintf("%d,", w / 2);
  }
}
MSH_CMD_EXPORT(get_half_width, get_half_width);

void get_all_width()
{
  for (int i = 0; i < 120; i++)
  {
    int w = f1.rightline[i] - f1.leftline[i];
    rt_kprintf("%d", w);
  }
}
MSH_CMD_EXPORT(get_all_width, get_all_width);
//////////////////////////////////////////////////////////

// //判断输入的数组为err<scale的直线则返回1
// //scale: 1~4
// int is_straight_line(int start_index, int end_index, int16 line_[IMG_H], int scale)
// {
//   float k;
//   float error;

//   if (start_index == end_index)
//   {
//     return 1;
//   }

//   k = (float)(line_[start_index] - line_[end_index]) / (start_index - end_index); //斜率
//   for (int i = (start_index < end_index ? start_index : end_index);
//        i <= (start_index < end_index ? end_index : start_index); i++)
//   {
//     error = line_[i] - (k * (i - start_index) + line_[start_index]);
//     if (fabs(error) >= scale)
//       return 0;
//   }

//   return 1;
// }

// //判断输入的数组为连续的直线则返回1
// //scale: 1~3
// int is_continuous_line(int start, int end, int16 src_line[IMG_H], int threshold)
// {
//   if (start == end)
//     return 0;

//   if (start > end)
//   {
//     int tmp1 = end;
//     end = start;
//     start = tmp1;
//   }

//   //int cnt = 0;
//   for (int i = start; i < end - 1; i++)
//   {
//     //threshold<变化<10,个数不多于3
//     if (abs(src_line[i] - src_line[i + 1]) > threshold)
//     {
//       return 0;
//       // if (abs(src_line[i] - src_line[i + 1]) > 10)
//       // {
//       //   return 0;
//       // }
//       // else
//       // {
//       //   cnt++;
//       //   if (cnt >= 3)
//       //     return 0;
//       // }
//     }
//   }

//   return 1;
// }

// //判断一个矩形区域内某颜色的占比大于threshold则返回1
// //color 0:black;255:white
// //(x1,y1) to (x2,y2)
// //threshold: 0-100
// int is_region_one_color(int x1, int y1, int x2, int y2, int color, int threshold)
// {
//   int total_cnt = 0;
//   int white_cnt = 0;
//   for (int i = x1; i <= x2; i++)
//   {
//     for (int j = y1; j <= y2; j++)
//     {
//       total_cnt++;
//       if (IMG_120_160[j][i] == color)
//         white_cnt++;
//     }
//   }
//   int per = white_cnt * 100 / total_cnt;
//   if (per > threshold)
//     return 1;
//   else
//     return 0;
// }

// //分别判断是否丢线
// //mode 0:left,1:right,2:both
// //threshold: 0-100
// //is_L_R_lose(40,120,2);
// //i=40,i=120-1
// int is_L_R_lose(int start, int end, int mode, int threshold)
// {
//   if (start == end)
//     return 0;

//   if (start > end)
//   {
//     int tmp1 = end;
//     end = start;
//     start = tmp1;
//   }

//   int lost_cnt = 0;
//   int total_cnt = 0;

//   for (int i = start; i < end; i++)
//   {
//     total_cnt++;

//     if (mode == 0)
//     {
//       if (f1.leftlineflag[i] == 0)
//       {
//         lost_cnt++;
//       }
//     }
//     else if (mode == 1)
//     {
//       if (f1.rightlineflag[i] == 0)
//       {
//         lost_cnt++;
//       }
//     }
//     else
//     {
//       if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0)
//       {
//         lost_cnt++;
//       }
//     }
//   }
//   if (lost_cnt * 100 / total_cnt >= threshold)
//     return 1;
//   else
//     return 0;
// }

// //分别判断是否丢线
// //mode 0:left,1:right,2:both
// //threshold: 0-100
// //is_L_R_nonlose(40,120,2);
// //i=40,i=120-1
// int is_L_R_nonlose(int start, int end, int mode, int threshold)
// {
//   if (start == end)
//     return 0;

//   if (start > end)
//   {
//     int tmp1 = end;
//     end = start;
//     start = tmp1;
//   }

//   int nonlost_cnt = 0;
//   int total_cnt = 0;

//   for (int i = start; i < end; i++)
//   {
//     total_cnt++;

//     if (mode == 0)
//     {
//       if (f1.leftlineflag[i] == 1)
//       {
//         nonlost_cnt++;
//       }
//     }
//     else if (mode == 1)
//     {
//       if (f1.rightlineflag[i] == 1)
//       {
//         nonlost_cnt++;
//       }
//     }
//     else
//     {
//       if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
//       {
//         nonlost_cnt++;
//       }
//     }
//   }
//   if (nonlost_cnt * 100 / total_cnt >= threshold)
//     return 1;
//   else
//     return 0;
// }

// int is_M_center(int start, int end, int half_err)
// {
//   if (start == end)
//     return 0;

//   if (start > end)
//   {
//     int tmp1 = end;
//     end = start;
//     start = tmp1;
//   }

//   for (int i = start; i < end; i++)
//   {
//     if (f1.midline[i] > 80 + half_err ||
//         f1.midline[i] < 80 - half_err)
//     {
//       return 0;
//     }
//   }
//   return 1;
// }

// //计算一个矩形区域内某颜色的占比
// //color 0:black;255:white
// //(x1,y1) to (x2,y2)
// //return: 0-100
// int solve_region_duty(int x1, int y1, int x2, int y2, int color)
// {
//   int total_cnt = 0;
//   int white_cnt = 0;
//   for (int i = x1; i <= x2; i++)
//   {
//     for (int j = y1; j <= y2; j++)
//     {
//       total_cnt++;
//       if (IMG_120_160[j][i] == color)
//         white_cnt++;
//     }
//   }
//   int per = white_cnt * 100 / total_cnt;
//   return per;
// }

// float solve_variance(int begin_row, int end_row)
// {
//   if (begin_row == end_row)
//     return 0;

//   if (begin_row < end_row)
//   {
//     int tmp1 = begin_row;
//     begin_row = end_row;
//     end_row = tmp1;
//   }

//   int center_sum = 0;
//   int count_center = 0;
//   int center_ave = 0;
//   int quadratic_sum = 0;

//   for (int i = begin_row; i > end_row; i--)
//   {
//     center_sum += f1.midline[i];
//     count_center++;
//   }

//   center_ave = (int)((float)center_sum / (float)count_center); //求得平均中值

//   for (int i = begin_row; i > end_row; i--)
//   {
//     quadratic_sum += (f1.midline[i] - center_ave) * (f1.midline[i] - center_ave); //每行据平均中值得平方和
//   }

//   float variance = (float)quadratic_sum / (float)count_center;
//   return (variance);
// }

// float solve_slope(int begin, int end, int16 *p)
// {
//   if (begin == end)
//     return 0;

//   if (begin > end)
//   {
//     int tmp1 = end;
//     end = begin;
//     begin = tmp1;
//   }

//   float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
//   uint8 i = 0;
//   float result = 0;
//   static float resultlast;
//   p = p + begin;
//   for (i = begin; i < end; i++)
//   {
//     xsum += i;
//     ysum += ((float)(*p));
//     xysum += i * ((float)(*p));
//     x2sum += i * i;
//     p = p + 1;
//   }
//   if ((end - begin) * x2sum - xsum * xsum)
//   {
//     result = (float)((end - begin) * xysum - xsum * ysum) / (float)((end - begin) * x2sum - xsum * xsum);
//     resultlast = result;
//   }
//   else
//   {
//     result = resultlast;
//   }
//   return result;
// }
