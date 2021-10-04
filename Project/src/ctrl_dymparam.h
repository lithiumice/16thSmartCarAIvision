#ifndef __DYM_H__
#define __DYM_H__
#include <rtthread.h>
using namespace rtthread;
#include "main.h"
#include "hw_config.h"
#include "common_macro.h"
#include "headfile.h"
#include "img_proc.h"
#include "f_elems.h"
#include "string"

class dymparam_ctrl
{
public:
    int16 road_type[200];
    float speed_set = 0;
    float speed_real = 0;
    int16 foresight = 0;
    int16 fore_fix = 72;
    int16 fore_max = 100;
    float dym_p1 = 32;  //24
    float dym_p2 = 0.4; //0.75

    typedef enum
    {
        LONG_STRAIGHT,
        SHORT_STRAIGHT,
        SMALL_CURVE,
        LARGE_CURVE,
        LOOP,
        CROSS,
        S_CROSS,
        FORK,
        GARBAGE,
        TAG,
        START,
        STOPED,
        RAMP,
        NONE
    } road;

    std::string road_name[14] = {
        "long",
        "short",
        "S_curve",
        "L_curve",
        "loop",
        "cross",
        "s_cross",
        "fork",
        "garage",
        "qrcode",
        "start",
        "stoped",
        "ramp",
        "none"};

    float const_speed = 270;

#ifdef USE_CAR_1
    // int16 s_max_speed = 360;
    // int16 s_min_speed = 260;
    // int16 fore_min = 64;
    int16 s_max_speed = 340;
    int16 s_min_speed = 280;
    int16 fore_min = 67;
#else
    int16 s_max_speed = 350;
    int16 s_min_speed = 250;
    int16 fore_min = 68;
#endif

    dymparam_ctrl(){};

    void dym_forsight();

    void dym_type_speed();

    void dym_curve_speed(float h, float l, int p);
};
extern dymparam_ctrl dymparam;

#endif