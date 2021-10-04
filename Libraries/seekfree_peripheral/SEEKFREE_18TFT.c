/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		TFT
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SCL                 查看TFT_SCL宏定义的引脚     硬件SPI引脚不可随意切换
						SDA                 查看TFT_SDA宏定义的引脚     硬件SPI引脚不可随意切换
						RES                 查看REST_PIN宏定义的引脚    
						DC                  查看DC_PIN宏定义的引脚  
						CS                  查看TFT_CS宏定义的引脚      硬件SPI引脚不可随意切换
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
						最大分辨率128*160
					------------------------------------ 

 ********************************************************************************************************************/

#include "zf_spi.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_systick.h"
#include "SEEKFREE_PRINTF.h"
#include "SEEKFREE_18TFT.h"

int TFT_PENCOLOR = RED;
int TFT_BGCOLOR = WHITE;

void lcd_writeIndex(uint8 dat) //写命令
{
    DC(0);
    spi_mosi(TFT_SPIN, TFT_CS, &dat, &dat, 1, 1);
}

void lcd_writeData(uint8 dat) //写数据
{
    DC(1);
    spi_mosi(TFT_SPIN, TFT_CS, &dat, &dat, 1, 1);
}

void lcd_writedata_16bit(uint16 dat) //向液晶屏写一个16位数据
{
    uint8 dat1[2];
    dat1[0] = dat >> 8;
    dat1[1] = (uint8)dat;

    DC(1);
    spi_mosi(TFT_SPIN, TFT_CS, dat1, dat1, 2, 1); //写入数据  高位在前  低位在后
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶坐标设置
//  @param      x_start     	坐标x方向的起点
//  @param      y_start     	坐标y方向的起点
//  @param      x_end       	坐标x方向的终点
//  @param      y_end       	坐标y方向的终点
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_set_region(0,0,10,10);// x、y的起点都是0，终点都是10
//-------------------------------------------------------------------------------------------------------------------
void lcd_set_region(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end)
{
    //assert(TFT_X_MAX > x_start); //参数断言，进入断言标志参数超出范围
    //assert(TFT_Y_MAX > y_start);

    //assert(TFT_X_MAX > x_end);
    //assert(TFT_Y_MAX > y_end);

#if (0 == TFT_DISPLAY_DIR || 1 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(x_start + 2);
    lcd_writeData(0x00);
    lcd_writeData(x_end + 2);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(y_start + 1);
    lcd_writeData(0x00);
    lcd_writeData(y_end + 1);

#elif (2 == TFT_DISPLAY_DIR || 3 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(x_start + 1);
    lcd_writeData(0x0);
    lcd_writeData(x_end + 1);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(y_start + 2);
    lcd_writeData(0x00);
    lcd_writeData(y_end + 2);

#endif

    lcd_writeIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_clear(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void lcd_clear(int color)
{
    uint8 i, j;
    lcd_set_region(0, 0, TFT_X_MAX - 1, TFT_Y_MAX - 1);
    for (i = 0; i < TFT_Y_MAX; i++)
        for (j = 0; j < TFT_X_MAX; j++)
            lcd_writedata_16bit(color);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{
    spi_init(TFT_SPIN, TFT_SCL, TFT_SDA, TFT_SDA_IN, TFT_CS, 0, 30 * 1000 * 1000); //硬件SPI初始化

    gpio_init(BL_PIN, GPO, 1, GPIO_PIN_CONFIG);
    gpio_init(DC_PIN, GPO, 0, GPIO_PIN_CONFIG);
    gpio_init(REST_PIN, GPO, 0, GPIO_PIN_CONFIG);

    REST(0);
    systick_delay_ms(10);
    REST(1);
    systick_delay_ms(120);
    lcd_writeIndex(0x11);
    systick_delay_ms(120);

    lcd_writeIndex(0xB1);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB2);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB3);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);
    lcd_writeData(0x01);
    lcd_writeData(0x2C);
    lcd_writeData(0x2D);

    lcd_writeIndex(0xB4);
    lcd_writeData(0x07);

    lcd_writeIndex(0xC0);
    lcd_writeData(0xA2);
    lcd_writeData(0x02);
    lcd_writeData(0x84);
    lcd_writeIndex(0xC1);
    lcd_writeData(0xC5);

    lcd_writeIndex(0xC2);
    lcd_writeData(0x0A);
    lcd_writeData(0x00);

    lcd_writeIndex(0xC3);
    lcd_writeData(0x8A);
    lcd_writeData(0x2A);
    lcd_writeIndex(0xC4);
    lcd_writeData(0x8A);
    lcd_writeData(0xEE);

    lcd_writeIndex(0xC5);
    lcd_writeData(0x0E);

    lcd_writeIndex(0x36);
    switch (TFT_DISPLAY_DIR) //y x v
    {
    case 0:
        lcd_writeData(1 << 7 | 1 << 6 | 0 << 5);
        break; //竖屏模式
    case 1:
        lcd_writeData(0 << 7 | 0 << 6 | 0 << 5);
        break; //竖屏模式  旋转180
    case 2:
        lcd_writeData(1 << 7 | 0 << 6 | 1 << 5);
        break; //横屏模式
    case 3:
        lcd_writeData(0 << 7 | 1 << 6 | 1 << 5);
        break; //横屏模式  旋转180
    }

    lcd_writeIndex(0xe0);
    lcd_writeData(0x0f);
    lcd_writeData(0x1a);
    lcd_writeData(0x0f);
    lcd_writeData(0x18);
    lcd_writeData(0x2f);
    lcd_writeData(0x28);
    lcd_writeData(0x20);
    lcd_writeData(0x22);
    lcd_writeData(0x1f);
    lcd_writeData(0x1b);
    lcd_writeData(0x23);
    lcd_writeData(0x37);
    lcd_writeData(0x00);
    lcd_writeData(0x07);
    lcd_writeData(0x02);
    lcd_writeData(0x10);

    lcd_writeIndex(0xe1);
    lcd_writeData(0x0f);
    lcd_writeData(0x1b);
    lcd_writeData(0x0f);
    lcd_writeData(0x17);
    lcd_writeData(0x33);
    lcd_writeData(0x2c);
    lcd_writeData(0x29);
    lcd_writeData(0x2e);
    lcd_writeData(0x30);
    lcd_writeData(0x30);
    lcd_writeData(0x39);
    lcd_writeData(0x3f);
    lcd_writeData(0x00);
    lcd_writeData(0x07);
    lcd_writeData(0x03);
    lcd_writeData(0x10);

    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(0x00 + 2);
    lcd_writeData(0x00);
    lcd_writeData(0x80 + 2);

    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(0x00 + 3);
    lcd_writeData(0x00);
    lcd_writeData(0x80 + 3);

    lcd_writeIndex(0xF0);
    lcd_writeData(0x01);
    lcd_writeIndex(0xF6);
    lcd_writeData(0x00);

    lcd_writeIndex(0x3A);
    lcd_writeData(0x05);
    lcd_writeIndex(0x29);

    lcd_clear(WHITE); //全白
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x     	        坐标x方向的起点
//  @param      y     	        坐标y方向的起点
//  @param      dat       	    需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_drawpoint(0,0,RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void lcd_drawpoint(uint16 x, uint16 y, uint16 color)
{
    lcd_set_region(x, y, x, y);
    lcd_writedata_16bit(color);
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void lcd_fill(uint16 sx, uint16 sy, uint16 ex, uint16 ey, uint16 color)
{
    uint16 i, j;
    uint16 width = ex - sx + 1;     //得到填充的宽度
    uint16 height = ey - sy + 1;    //高度
    lcd_set_region(sx, sy, ex, ey); //设置显示窗口
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
            lcd_writedata_16bit(color); //写入数据
    }
    lcd_set_region(0, 0, TFT_X_MAX - 1, TFT_Y_MAX - 1); //恢复窗口设置为全屏
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void lcd_drawline(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
{
    uint16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        lcd_drawpoint(uRow, uCol, color); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/
void _draw_circle_8(int xc, int yc, int x, int y, uint16 c)
{
    lcd_drawpoint(xc + x, yc + y, c);

    lcd_drawpoint(xc - x, yc + y, c);

    lcd_drawpoint(xc + x, yc - y, c);

    lcd_drawpoint(xc - x, yc - y, c);

    lcd_drawpoint(xc + y, yc + x, c);

    lcd_drawpoint(xc - y, yc + x, c);

    lcd_drawpoint(xc + y, yc - x, c);

    lcd_drawpoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,u16 c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/
void gui_circle(int xc, int yc, uint16 c, int r, int fill)
{
    int x = 0, y = r, yi, d;

    d = 3 - 2 * r;

    if (fill)
    {
        // 如果填充（画实心圆）
        while (x <= y)
        {
            for (yi = x; yi <= y; yi++)
                _draw_circle_8(xc, yc, x, yi, c);

            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
    else
    {
        // 如果不填充（画空心圆）
        while (x <= y)
        {
            _draw_circle_8(xc, yc, x, y, c);
            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void lcd_showchar(uint16 x, uint16 y, const int8 dat)
{
    uint8 i, j;
    uint8 temp;

    for (i = 0; i < 16; i++)
    {
        lcd_set_region(x, y + i, x + 7, y + i);
        temp = tft_ascii[dat - 32][i]; //减32因为是取模是从空格开始取得 空格在ascii中序号是32
        for (j = 0; j < 8; j++)
        {
            if (temp & 0x01)
                lcd_writedata_16bit(TFT_PENCOLOR);
            else
                lcd_writedata_16bit(TFT_BGCOLOR);
            temp >>= 1;
        }
    }
}

/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
                fc:the color value of display character
                bc:the background color of display character
                num:the ascii code of display character(0~94)
                size:the size of display character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void lcd_flexchar(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 num, uint8 size, uint8 mode)
{
    uint8 temp;
    uint8 pos, t;
    uint16 colortemp = TFT_PENCOLOR;

    num = num - ' ';
    lcd_set_region(x, y, x + size / 2 - 1, y + size - 1);
    if (!mode)
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
                temp = asc2_1206[num][pos];
            else
                temp = asc2_1608[num][pos];
            for (t = 0; t < size / 2; t++)
            {
                if (temp & 0x01)
                    lcd_writedata_16bit(fc);
                else
                    lcd_writedata_16bit(bc);
                temp >>= 1;
            }
        }
    }
    else //??????
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
                temp = asc2_1206[num][pos];
            else
                temp = asc2_1608[num][pos];
            for (t = 0; t < size / 2; t++)
            {
                TFT_PENCOLOR = fc;
                if (temp & 0x01)
                    lcd_drawpoint(x + t, y + pos, fc);
                temp >>= 1;
            }
        }
    }
    TFT_PENCOLOR = colortemp;
    lcd_set_region(0, 0, TFT_X_MAX - 1, TFT_Y_MAX - 1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void lcd_showstr(uint16 x, uint16 y, const int8 dat[])
{
    uint16 j;

    j = 0;
    while (dat[j] != '\0')
    {
        lcd_flexchar(x + 6 * j, y * 12, RED, WHITE, dat[j], 12, 0);
        // lcd_showchar(x + 8 * j, y * 16, dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_flexstr(0,0,RED,WHITE,"seekfree",0);
//-------------------------------------------------------------------------------------------------------------------
void lcd_flexstr(uint16 x, uint16 y, uint16 fc, uint16 bc, const int8 dat[], uint8 mode)
{
    uint16 j;

    j = 0;
    while (dat[j] != '\0')
    {
        //12*6 char
        //x: 160/6=26
        //y: 120/12=10
        lcd_flexchar(x + 6 * j, y * 12, fc, bc, dat[j], 12, mode);
        j++;
    }
}

void lcd_big_str(uint16 x, uint16 y, uint16 fc, uint16 bc, const int8 dat[], uint8 mode)
{
    uint16 j;

    j = 0;
    while (dat[j] != '\0')
    {
        lcd_flexchar(x + 12 * j, y * 24, fc, bc, dat[j], 24, mode);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位有符号
//  @param      x     	        坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int8
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint8(0,0,x);//x为int8类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint8(uint16 x, uint16 y, int8 dat)
{
    uint8 a[3];
    uint8 i;
    if (dat < 0)
    {
        lcd_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else
        lcd_showchar(x, y * 16, ' ');

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while (i < 3)
    {
        lcd_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint8(0,0,x);//x为uint8类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint8(uint16 x, uint16 y, uint8 dat)
{
    uint8 a[3];
    uint8 i;

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while (i < 3)
    {
        lcd_showchar(x + (8 * i), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位有符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int16
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint16(0,0,x);//x为int16类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint16(uint16 x, uint16 y, int16 dat)
{
    uint8 a[5];
    uint8 i;
    if (dat < 0)
    {
        lcd_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else
        lcd_showchar(x, y * 16, ' ');

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while (i < 5)
    {
        lcd_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint16(0,0,x);//x为uint16类型
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint16(uint16 x, uint16 y, uint16 dat)
{
    uint8 a[5];
    uint8 i;

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while (i < 5)
    {
        lcd_showchar(x + (8 * i), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示32位有符号(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint32
//  @param      num       	    需要显示的位数 最高10位  不包含正负号
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint32(0,0,x,3);//x可以为int32 uint16 int16 uint8 int8类型
//  Sample usage:               负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint32(uint16 x, uint16 y, int32 dat, uint8 num)
{
    int8 buff[34];
    uint8 length;

    if (10 < num)
        num = 10;

    num++;
    if (0 > dat)
        length = zf_sprintf(&buff[0], "%d", dat); //负数
    else
    {
        buff[0] = ' ';
        length = zf_sprintf(&buff[1], "%d", dat);
        length++;
    }
    while (length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    lcd_showstr(x, y, buff); //显示数字
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示浮点数(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型float或double
//  @param      num       	    整数位显示长度   最高10位
//  @param      pointnum        小数位显示长度   最高6位
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showfloat(0,0,x,2,3);//显示浮点数   整数显示2位   小数显示三位
//  @note                       特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void lcd_showfloat(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum)
{
    uint8 length;
    int8 buff[34];
    int8 start, end, point;

    if (6 < pointnum)
        pointnum = 6;
    if (10 < num)
        num = 10;

    if (0 > dat)
        length = zf_sprintf(&buff[0], "%f", dat); //负数
    else
    {
        length = zf_sprintf(&buff[1], "%f", dat);
        length++;
    }
    point = length - 7;         //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位
    while (0 > start)           //整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if (0 > dat)
        buff[start] = '-';
    else
        buff[start] = ' ';

    buff[end] = '\0';

    lcd_showstr(x, y, buff); //显示数字
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示40*40 QQ图片
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void showimage(const unsigned char *p)
{
    int i, j, k;
    unsigned char picH, picL;
    lcd_clear(WHITE); //清屏

    for (k = 0; k < 4; k++)
    {
        for (j = 0; j < 3; j++)
        {
            lcd_set_region(40 * j, 40 * k, 40 * j + 39, 40 * k + 39); //坐标设置
            for (i = 0; i < 40 * 40; i++)
            {
                picL = *(p + i * 2); //数据低位在前
                picH = *(p + i * 2 + 1);
                lcd_writedata_16bit(picH << 8 | picL);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H)//显示灰度摄像头 图像
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage032(uint8 *p, uint16 width, uint16 height)
{
    uint32 i, j;

    uint16 color = 0;
    uint16 temp = 0;

    uint16 coord_x = 0;
    uint16 coord_y = 0;

    if (0 == TFT_DISPLAY_DIR || 1 == TFT_DISPLAY_DIR) //竖屏
    {
        coord_x = height > TFT_X_MAX ? TFT_X_MAX : height;
        coord_y = width > TFT_Y_MAX ? TFT_Y_MAX : width;

        for (j = 0; j < coord_y; j++)
        {
            lcd_set_region(0, j, coord_x - 1, j);
            for (i = 0; i < coord_x; i++)
            {
                temp = *(p + i * width + j * width / coord_y); //读取像素点
                color = (0x001f & ((temp) >> 3)) << 11;
                color = color | (((0x003f) & ((temp) >> 2)) << 5);
                color = color | (0x001f & ((temp) >> 3));
                lcd_writedata_16bit(color);
            }
        }
    }
    else //横屏
    {
        coord_x = width > TFT_X_MAX ? TFT_X_MAX : width;
        coord_y = height > TFT_Y_MAX ? TFT_Y_MAX : height;
        lcd_set_region(0, 0, coord_x - 1, coord_y - 1);

        for (j = 0; j < coord_y; j++)
        {
            for (i = 0; i < coord_x; i++)
            {
                temp = *(p + j * width + i * width / coord_x); //读取像素点
                color = (0x001f & ((temp) >> 3)) << 11;
                color = color | (((0x003f) & ((temp) >> 2)) << 5);
                color = color | (0x001f & ((temp) >> 3));
                lcd_writedata_16bit(color);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶缩放显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @param      dis_width       图像显示宽度  1 -（TFT_X_MAX）
//  @param      dis_height      图像显示高度  1 -（TFT_Y_MAX）
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, 160, 120)//显示灰度摄像头 图像
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i, j;

    uint16 color = 0;
    uint16 temp = 0;

    lcd_set_region(0, 0, dis_width - 1, dis_height - 1); //设置显示区域

    for (j = 0; j < dis_height; j++)
    {
        for (i = 0; i < dis_width; i++)
        {
            temp = *(p + (j * height / dis_height) * width + i * width / dis_width); //读取像素点
            color = (0x001f & ((temp) >> 3)) << 11;
            color = color | (((0x003f) & ((temp) >> 2)) << 5);
            color = color | (0x001f & ((temp) >> 3));
            lcd_writedata_16bit(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      凌瞳(彩色摄像头)液晶缩放显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @param      dis_width       图像显示宽度  1 -（TFT_X_MAX）
//  @param      dis_height      图像显示高度  1 -（TFT_Y_MAX）
//  @return     void
//  @since      v1.0
//  Sample usage:              	lcd_displayimage8660_zoom(scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H,160,128);
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage8660_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i, j;

    uint16 color = 0;

    lcd_set_region(0, 0, dis_width - 1, dis_height - 1); //设置显示区域

    for (j = 0; j < dis_height; j++)
    {
        for (i = 0; i < dis_width; i++)
        {
            color = *(p + (j * height / dis_height) * width + i * width / dis_width); //读取像素点
            color = ((color & 0xff) << 8) | (color >> 8);
            lcd_writedata_16bit(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风(二值化摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @return     void
//  @since      v1.0
//  @note      	图像分辨率需要设置在120(高)*160(宽)以下
//-------------------------------------------------------------------------------------------------------------------
void lcd_displayimage7725(uint8 *p, uint16 width, uint16 height)
{
    int i, j;

    uint16 temp = 0;
    lcd_set_region(0, 0, width - 1, height - 1);
    for (i = 0; i < height * (width / 8); i++)
    {
        temp = *p;
        p++;
        for (j = 0; j < 8; j++)
        {
            if ((temp << j) & 0x80)
                lcd_writedata_16bit(WHITE);
            else
                lcd_writedata_16bit(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      汉字显示
//  @param      x       横坐标 0-（TFT_X_MAX-1）
//  @param      y       纵坐标 0-（TFT_Y_MAX-1）
//  @param      size    取模的时候设置的汉字字体大小，也就是一个汉字占用的点阵长宽为多少个点，取模的时候需要长宽是一样的。
//  @param      *p      需要显示的汉字数组
//  @param      number  需要显示多少位
//  @param      color   显示颜色
//  @return     void
//  @since      v1.0
//  Sample usage:		lcd_display_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
//  @Note       使用PCtoLCD2002软件取模		    阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void lcd_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k;
    uint8 temp, temp1, temp2;
    const uint8 *p_data;

    temp2 = size / 8;

    lcd_set_region(x, y, number * size - 1 + x, y + size - 1);

    for (i = 0; i < size; i++)
    {
        temp1 = number;
        p_data = p + i * temp2;
        while (temp1--)
        {
            for (k = 0; k < temp2; k++)
            {
                for (j = 8; j > 0; j--)
                {
                    temp = (*p_data >> (j - 1)) & 0x01;
                    if (temp)
                        lcd_writedata_16bit(color);
                    else
                        lcd_writedata_16bit(TFT_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }
    }
}
