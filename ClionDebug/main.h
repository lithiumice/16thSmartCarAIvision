//
// Created by lithiumice on 2021/5/30.
//

#ifndef CAR_MAIN_H
#define CAR_MAIN_H

#define CLION_DEBUG
#define LONG_STRAIGHT_ENDROW 40
#define ELE_SWITCH 8
#define uint8_t uint8
#define uint16_t uint16
#define uint32_t uint32
#define int8_t int8
#define int16_t int16
#define int32_t int32

#define ENABLE_CROSS
#define ENABLE_CURVE
#define ENABLE_LOOP
#define ENABLE_FORK
#define ENABLE_QRCODE

#define ENABLE_START
#define ENABLE_GARAGE
#define ENABLE_RAMP

#define rt_tick_t int
void chassis_bake();
class inoutdev_ctrl
{
public:
    float gyro_z_angle = 0;
    float gyro_y_angle = 0;
    float start_angle = 0;
    float cross_z_angle;

    void beep(int num);
    static void lbeep(int num);
    int switch_is_on(int num);
};

class chassis_ctrl
{
public:
    float allele_distance=0;
    float all_distance = 0;
    float loop_distance = 0;
    float fork_distance = 0;
    float tag_distance = 0;
    float start_distance = 0;
    float ramp_distance = 0;
    float fork_cnt_distance;

float row_to_distance(int row){return 1;};

void set_speed(float spe,int ang){};
};

extern chassis_ctrl chassis;
extern inoutdev_ctrl inoutdev;

#endif //CAR_MAIN_H
