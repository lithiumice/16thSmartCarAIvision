#ifndef __LQ_LED_H__
#define __LQ_LED_H__

//#include <iostream>
#include "rtthread.h"
using namespace rtthread;
#include "headfile.h"

#define LED1_PIN C25
#define LED2_PIN C26
#define LED3_PIN C27

class mix_leds
{
public:
  typedef enum
  {
    white = 0,  //白色
    black = 1,  //黑色
    red = 2,    //红
    green = 3,  //绿
    blue = 4,   //蓝色
    yellow = 5, //黄色
    violet = 6, //紫色
    cyan = 7,   //青色
  } LED_t;

  mix_leds() {}

  void init()
  {
    gpio_init(LED1_PIN, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
    gpio_init(LED2_PIN, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
    gpio_init(LED3_PIN, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
  }

  void set(LED_t color)
  {
    switch (color)
    {
    case white:
      gpio_set(LED1_PIN, 0);
      gpio_set(LED2_PIN, 0);
      gpio_set(LED3_PIN, 0);
      break;
    case black:
      gpio_set(LED1_PIN, 1);
      gpio_set(LED2_PIN, 1);
      gpio_set(LED3_PIN, 1);
      break;
    case red:
      gpio_set(LED1_PIN, 1);
      gpio_set(LED2_PIN, 0);
      gpio_set(LED3_PIN, 1);
      break;
    case green:
      gpio_set(LED1_PIN, 0);
      gpio_set(LED2_PIN, 1);
      gpio_set(LED3_PIN, 1);
      break;
    case blue:
      gpio_set(LED1_PIN, 1);
      gpio_set(LED2_PIN, 1);
      gpio_set(LED3_PIN, 0);
      break;
    case yellow:
      gpio_set(LED1_PIN, 0);
      gpio_set(LED2_PIN, 0);
      gpio_set(LED3_PIN, 1);
      break;
    case violet:
      gpio_set(LED1_PIN, 1);
      gpio_set(LED2_PIN, 0);
      gpio_set(LED3_PIN, 0);
      break;
    case cyan:
      gpio_set(LED1_PIN, 0);
      gpio_set(LED2_PIN, 1);
      gpio_set(LED3_PIN, 0);
      break;
    }
  }

  void reverse(LED_t color)
  {
    static uint8_t count = 0;
    if (count++ % 2)
    {
      set(color);
    }
    else
    {
      set(black);
    }
  }
};

#endif
