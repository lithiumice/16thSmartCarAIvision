#ifndef __PSTWO_H
#define __PSTWO_H
#include <rtthread.h>
#include "common.h"
using namespace rtthread;

#define PS2_DAT_PIN C7
#define PS2_CMD_PIN C6
#define PS2_CS_PIN C16
#define PS2_CLK_PIN C17

#define PS2_DAT gpio_get(PS2_DAT_PIN)       //DATA
#define PS2_CMD(x) gpio_set(PS2_CMD_PIN, x) //CMD
#define PS2_CS(x) gpio_set(PS2_CS_PIN, x)   //CS
#define PS2_CLK(x) gpio_set(PS2_CLK_PIN, x) //CLK

#define PSB_SELECT 1
#define PSB_L3 2
#define PSB_R3 3
#define PSB_START 4
#define PSB_PAD_UP 5
#define PSB_PAD_RIGHT 6
#define PSB_PAD_DOWN 7
#define PSB_PAD_LEFT 8
#define PSB_L2 9
#define PSB_R2 10
#define PSB_L1 11
#define PSB_R1 12
#define PSB_GREEN 13
#define PSB_RED 14
#define PSB_BLUE 15
#define PSB_PINK 16
#define PSB_TRIANGLE 13
#define PSB_CIRCLE 14
#define PSB_CROSS 15
#define PSB_SQUARE 26

void PS2_Init(void);
void PS2_Cmd(uint8 cmd);
void PS2_Read(void);
void PS2_DataClear(void);
uint8 PS2_DataKey(void);

#endif