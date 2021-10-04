#ifndef __HW_CONFIG__
#define __HW_CONFIG__
#include "headfile.h"

#define USE_CAR_1
// #define USE_CAR_2
// #define USE_CAR_3

#ifdef USE_CAR_1
///////////////////////////////////////////////////////
//car 1-----------------------------------------------
///////////////////////////////////////////////////////
#define SERVO1_MID_POS 708                 //中值变小，往右
#define SERVO1_L_POS (SERVO1_MID_POS - 70) //right
#define SERVO1_R_POS (SERVO1_MID_POS + 70) //left
#define SERVO1_MAX_ANGLE 40
#define PULSE_PER_1CM 115.0f

#elif defined(USE_CAR_2)
///////////////////////////////////////////////////////
//car 2-----------------------------------------------
///////////////////////////////////////////////////////
#define SERVO1_MID_POS 725
#define SERVO1_L_POS (SERVO1_MID_POS - 65)
#define SERVO1_R_POS (SERVO1_MID_POS + 65)
#define SERVO1_MAX_ANGLE 35
#define PULSE_PER_1CM 115.0f

#elif defined(USE_CAR_3)
///////////////////////////////////////////////////////
//car 3-----------------------------------------------
///////////////////////////////////////////////////////
#define SERVO1_MID_POS 708                 //中值变小，往右
#define SERVO1_L_POS (SERVO1_MID_POS - 70) //right
#define SERVO1_R_POS (SERVO1_MID_POS + 70) //left
#define SERVO1_MAX_ANGLE 40
#define PULSE_PER_1CM 57.5f

#endif

///////////////////////////////////////////////////////
//common-----------------------------------------------
///////////////////////////////////////////////////////
//remote
#define REMOTE_PIN1 C6
#define REMOTE_PIN2 C7

//servo
#define SERVO1_PIN PWM4_MODULE2_CHA_C30
#define SERVO2_PIN PWM4_MODULE3_CHA_C31

#define SERVO2_MID_POS 750
#define SERVO2_L_POS 250
#define SERVO3_R_POS 1250

#define SERVO1_INVERSE 0
#define SERVO2_INVERSE 0
#define SERVO1_HALF_ONE 70
#define SERVO2_HALF_ONE 500

//switch
#define SWITCH_8 C26
#define SWITCH_7 C25
#define SWITCH_6 C27 //ban
#define SWITCH_5 C27
#define SWITCH_4 C18
#define SWITCH_3 C15
#define SWITCH_2 B10
#define SWITCH_1 B11

//button
#define KEY1_PIN D27
#define KEY2_PIN D15
#define KEY3_PIN D14
#define KEY4_PIN D4 //+
#define KEY5_PIN B17
#define KEY6_PIN B21 //-
#define KEY7_PIN B19

//adc
#define ADC_L_PIN ADC1_CH5_B16
#define ADC_R_PIN ADC1_CH4_B15
#define ADC_BAT_PIN ADC1_CH3_B14

//elec
#define ADC_1_PIN ADC1_CH6_B17
#define ADC_2_PIN ADC1_CH7_B18
#define ADC_3_PIN ADC1_CH8_B19

//encoder
#define ENCODER_LEFT_INV 1
#define ENCODER_RIGHT_INV 0

#define ENCODER1_QTIMER QTIMER_1
#define ENCODER1_A QTIMER1_TIMER0_C0
#define ENCODER1_B QTIMER1_TIMER1_C1

#define ENCODER2_QTIMER QTIMER_1
#define ENCODER2_A QTIMER1_TIMER2_C2
#define ENCODER2_B QTIMER1_TIMER3_C24

//laser
#define LASER_PIN PWM1_MODULE0_CHB_D13
#define LED0_PIN B9
#define BEEP_PIN D26

#endif