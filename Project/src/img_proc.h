#ifndef __IMAGE_PROCESS
#define __IMAGE_PROCESS

#include "main.h"

#ifndef CLION_DEBUG

#include "common.h"
#include <rtthread.h>
using namespace rtthread;
#include "common_macro.h"

#else

#include "common.h"
#include "common_macro.h"

#endif
#include "common.h"

#define IMG_W 320
#define IMG_H 240

#define IMG_120_160 bin_120_160_image
#define IMG_240_320 bin_240_320_image

#define img_gray_width 320
#define img_gray_height 240

// #define FIX_BIN_THRESHOLD 90 //80,128

#define FIX_BIN_THRESHOLD 120 //80,128

typedef struct image
{
    // char *name;
    uint8 *data;
    uint32 width;
    uint32 height;
    uint32 step;
} image_t;

// typedef struct my_image
// {
//     const char *const name;
//     const int width;
//     const int height;
//     const uint8_t *buffer;
//     struct my_image *next;
// } my_image_t;

typedef struct my_param
{
    const char *const name;
    const float min_value;
    const float max_value;
    const float step_value;
    float *current_value;
    struct my_param *next;
} my_param_t;

typedef struct
{
    int16 start_row;
    int16 end_row;
    int16 right_col;
    int16 left_col;
    int16 middle_col;

    int16 origin_pix;
    int16 direction;
} Box_Margin;

typedef struct
{
    int8 exeist;
    int16 L_down_row;
    int16 L_down_col;
    int16 L_up_row;
    int16 L_up_col;
    int16 R_down_row;
    int16 R_down_col;
    int16 R_up_row;
    int16 R_up_col;
} Box_Border;

extern uint8_t bin_240_320_image[240][320];
extern uint8_t gray_240_320_image[240][320];
extern uint8_t gray_120_160_image[120][160];
extern uint8_t bin_120_160_image[120][160];
extern uint8_t tmp_120_160_image[120][160];

class imgproc_ctrl
{
public:
    uint8_t img_threshold = FIX_BIN_THRESHOLD;
    uint8_t fix_threshold = FIX_BIN_THRESHOLD;
    uint8_t HistoGram[256];

    void gray_to_binary(uint8_t src[120][160], uint8_t dst[120][160], uint8_t mode);
    void img_resize(uint8_t src[240][320], uint8_t dst[120][160]);
    int ostu_threshold(const uint8_t img[120][160]);
    void blur_points(int16 pts_in[120], int16 pts_out[120], int kernel);
    void dilate3(uint8_t src[120][160], uint8_t dst[120][160]);
    void erode3(uint8_t src[120][160], uint8_t dst[120][160]);

    void gray_to_binary(uint8_t **src, uint8_t **dst, int height, int width, uint8_t mode);
    void img_resize(uint8_t **src, uint8_t **dst, int src_height, int src_width, int scale);
    void blur_points(float pts_in[][2], int num, float pts_out[][2], int kernel);
    int ostu_threshold(uint8 **img, int height, int width);
    void copy_image(uint8_t **src, uint8_t **dst, int height, int width);

    int solve_ostu();
};

extern imgproc_ctrl img_proc_c;

#endif