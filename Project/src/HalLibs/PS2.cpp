#include "PS2.h"
#include "headfile.h"
#include "rthw.h"
#include "fsl_common.h"

uint16 Handkey;
uint8 Data[9] = {};
uint16 MASK[] = {
    PSB_SELECT,
    PSB_L3,
    PSB_R3,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK};

void delay_us(int x)
{
    pit_delay_us(PIT_CH1, x);
    // rt_thread_delay(x / 100 + 1);
    // rt_thread_delay(1);
    // rt_hw_us_delay(x);
    // SDK_DelayAtLeastUs(x, PIT_SOURCE_CLOCK);
}

void PS2_Init(void)
{
    gpio_init(PS2_DAT_PIN, GPI, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(PS2_CMD_PIN, GPO, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(PS2_CS_PIN, GPO, GPIO_HIGH, GPIO_PIN_CONFIG);
    gpio_init(PS2_CLK_PIN, GPO, GPIO_HIGH, GPIO_PIN_CONFIG);
}

void PS2_Cmd(uint8 cmd)
{
    uint16 i;
    for (i = 0x01; i < 0x100; i <<= 1) //8次循环
    {
        // PS2_CLK = 1; //发送高电平，告诉PS2接收器我要准备数据了
        PS2_CLK(1);
        if (i & cmd)
        {
            // PS2_CMD = 1;
            PS2_CMD(1);
        }
        else
            // PS2_CMD = 0;
            PS2_CMD(0);
        delay_us(10); //我要准备数据了的时间，如果没有这个的话就会在一个机器周期后进入低电平，接收器反应不过来。

        // PS2_CLK = 0; //告诉接收器，我数据准备好了，你可以读取了。
        PS2_CLK(0);
        delay_us(20);
    }
    // PS2_CLK = 1; //时钟拉高，不工作
    PS2_CLK(1);
}

void PS2_Read(void)
{
    volatile uint8 byte; //必须要用volatile关键词来定义。关键词的作用请自行百度。
    uint16 i;

    // PS2_CS = 0;    //CS拉低
    PS2_CS(0);
    PS2_Cmd(0x01); //开始命令
    PS2_Cmd(0x42); //请求数据
    for (byte = 2; byte < 9; byte++)
    {
        for (i = 0x01; i < 0x100; i <<= 1)
        {
            // PS2_CLK = 1;  //单片机发送高电平，告诉接收器要准备数据了
            PS2_CLK(1);
            delay_us(50); //接收器准备数据的时间
            // PS2_CLK = 0;  //发送低电平，告诉接收器 单片机要开始读数据了
            PS2_CLK(0);
            if (PS2_DAT)
                Data[byte] = i | Data[byte];
        }
    }
    // PS2_CS = 1; //CS拉高
    PS2_CS(1);
}

uint8 PS2_DataKey(void)
{
    uint8 index;
    PS2_DataClear(); //清空数组

    PS2_Read();                         //单片机读接收器的数据
    Handkey = (Data[4] << 8) | Data[3]; //根据数据意义对照表，定义一个16位的变量。

    for (index = 0; index < 16; index++)               //当我们按下遥控器的按键时，数据会传到Data[3]或者Data[4]来。我这里进行16次for循环，用来判断哪个按键按下了。
    {                                                  //例如：当按下了SELECT按键，Data[3]=11111110B。Handkey=1111 1111 1111 1110B。
        if ((Handkey & (1 << (MASK[index] - 1))) == 0) //当第一次进入循环，Handkey&(1<<(MASK[0]-1)))
            return index + 1;                          //-->Handkey&(1<<0)--->1111 1111 1111 1110B & 0000 0000 0000 0001=0
    }
    return 0;
}

void PS2_DataClear(void)
{
    uint8 i;
    for (i = 0; i < 9; i++)
    {
        Data[i] = 0x00;
    }
}

// 判断是否为红灯模式，0x41=模拟绿灯，0x73=模拟红灯
// 返回值；0，红灯模式
// 其他，其他模式
uint8 PS2_RedLight(void)
{
    //   CS_L;
    PS2_CS(0);

    // PS2_Cmd(Comd[0]); // 开始命令
    // PS2_Cmd(Comd[1]); // 请求数据
    PS2_Cmd(0x01); //开始命令
    PS2_Cmd(0x42); //请求数据
                   //   CS_H;
    PS2_CS(1);

    if (Data[1] == 0X73)
        return 0;
    else
        return 1;
}

// 得到一个摇杆的模拟量 范围 0~256
uint8 PS2_AnologData(uint8 button)
{
    return Data[button];
}

//手柄配置初始化：
void PS2_ShortPoll(void)
{
    //   CS_L;
    PS2_CS(0);

    delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x42);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x00);
    //   CS_H;
    PS2_CS(1);

    delay_us(16);
}

//进入配置
void PS2_EnterConfing(void)
{
    //   CS_L;
    PS2_CS(0);

    delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01);
    PS2_Cmd(0x00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    //   CS_H;
    PS2_CS(1);

    delay_us(16);
}

// 发送模式设置
void PS2_TurnOnAnalogMode(void)
{
    //   CS_L;
    PS2_CS(0);

    PS2_Cmd(0x01);
    PS2_Cmd(0x44);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01); //analog=0x01;digital=0x00 软件设置发送模式
    PS2_Cmd(0xEE); //Ox03 锁存设置，即不可通过按键“MODE ”设置模式。        //0xEE 不锁存软件设置，可通过按键“MODE ”设置模式。
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    //   CS_H;
    PS2_CS(1);

    delay_us(16);
}

// 振动设置
void PS2_VibrationMode(void)
{
    //   CS_L;
    PS2_CS(0);

    delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x4D);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0X01);
    //   CS_H;
    PS2_CS(1);

    delay_us(16);
}

// 完成并保存配置
void PS2_ExitConfing(void)
{
    //   CS_L;
    PS2_CS(0);

    delay_us(16);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    //   CS_H;
    PS2_CS(1);

    delay_us(16);
}

void PS2_SetInit(void)
{
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_EnterConfing();     // 进入配置模式
    PS2_TurnOnAnalogMode(); // “红绿灯”配置模式，并选择是否保存
    PS2_VibrationMode();    // 开启震动模式
    PS2_ExitConfing();      // 完成并保存配置
}

void PS2_Vibration(uint8 motor1, uint8 motor2)
{
    //    CS_L;
    PS2_CS(0);
    delay_us(16);
    PS2_Cmd(0x01); // 开始命令
    PS2_Cmd(0x42); // 请求数据
    PS2_Cmd(0X00);
    PS2_Cmd(motor1);
    PS2_Cmd(motor2);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    //    CS_H;
    PS2_CS(1);

    delay_us(16);
}