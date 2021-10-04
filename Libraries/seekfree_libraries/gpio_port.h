#ifndef __GPIO_PORT_H__
#define __GPIO_PORT_H__

#include "zf_gpio.h"

#define PIN_MODE_OUTPUT GPO
#define PIN_MODE_INPUT GPI
#define PIN_MODE_INPUT_PULLUP GPI

#define PIN_LOW 0
#define PIN_HIGH 1

#define rt_pin_mode(a, b) gpio_init(a, b, 0, GPIO_PIN_CONFIG)
#define rt_pin_write(a, b) gpio_set(a, b)
#define rt_pin_read(a) gpio_get(a)

// #define rt_pin_mode(pin_num, pin_dir) gpio_init(pin_num, pin_dir, 0, GPIO_PIN_CONFIG)
// #define rt_pin_write(pin_num, value) gpio_set(pin_num, value)
// #define rt_pin_read(pin_num) gpio_get(pin_num)

#endif