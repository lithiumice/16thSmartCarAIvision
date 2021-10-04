#include "img_proc.h"
#include "cstring"
#include "cstdlib"

// #define AT_IMAGE(img, x, y) ((img)->data[(y) * (img)->step + (x)])
// #define AT_IMAGE(img, x, y, width) ((*(img+x))[y])
#define AT_IMAGE(img, x, y, width) (*((uint8 *)img + width * y + x))
// #define AT_IMAGE_CLIP(img, x, y) AT_IMAGE(img, clip(x, 0, (img)->width - 1), clip(y, 0, (img)->height - 1))
#define AT AT_IMAGE
// #define AT_CLIP(img, x, y) AT_IMAGE((img), clip((x), 0, (img)->width - 1), clip((y), 0, (img)->height - 1));
// *((int*)array + LineSize*i + j);

#define DEF_IMAGE(ptr, w, h)                            \
    {                                                   \
        .data = ptr, .width = w, .height = h, .step = w \
    }
#define ROI_IMAGE(img, x1, y1, w, h)                                               \
    {                                                                              \
        .data = &AT_IMAGE(img, x1, y1), .width = w, .height = h, .step = img.width \
    }

#ifndef CLION_DEBUG
AT_SDRAM_SECTION_ALIGN(uint8_t bin_240_320_image[240][320], 64);
AT_SDRAM_SECTION_ALIGN(uint8_t gray_240_320_image[240][320], 64);

AT_SDRAM_SECTION_ALIGN(uint8_t gray_120_160_image[120][160], 64);
AT_SDRAM_SECTION_ALIGN(uint8_t bin_120_160_image[120][160], 64);
AT_SDRAM_SECTION_ALIGN(uint8_t tmp_120_160_image[120][160], 64);

#else
uint8_t bin_240_320_image[240][320];
uint8_t gray_240_320_image[240][320];

uint8_t gray_120_160_image[120][160];
uint8_t bin_120_160_image[120][160];
uint8_t tmp_120_160_image[120][160];
#endif

// image_t bin_120_160 = {
//     .name = "bin", .width = 160, .height = 120, .data = (uint8_t *)bin_120_160_image};
//image_t bin_120_160 = DEF_IMAGE((uint8_t *)bin_120_160_image, 160, 120);

// 点集三角滤波
//void imgproc_ctrl::blur_points(int16 pts_in[120], int16 pts_out[120], int kernel)
//{
//    assert(kernel % 2 == 1);
//    int half = kernel / 2;
//
//    for (int i = 0; i < 120; i++)
//    {
//        pts_out[i] = 0;
//        for (int j = -half; j <= half; j++)
//        {
//            pts_out[i] += pts_in[clip(i + j, 0, 120 - 1)] * (half + 1 - abs(j));
//        }
//        pts_out[i] /= (2 * half + 2) * (half + 1) / 2;
//    }
//}

// void clone_image(image_t* img0, image_t* img1){
//     assert(img0 && img0->data);
//     assert(img1 && img1->data);
//     assert(img0->width == img1->width && img0->height == img1->height);
//     assert(img0 != img1 && img0->data != img1->data);

//     if(img0->width == img0->step && img1->width == img1->step){
//         memcpy(img1->data, img0->data, img0->width*img0->height);
//     }else{
//         for(int y=0; y<img0->height; y++){
//             memcpy(&AT(img1, 0, y), &AT(img0, 0, y), img0->width);
//         }
//     }
// }
void imgproc_ctrl::copy_image(uint8_t **src, uint8_t **dst, int height, int width)
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            // dst[i][j] = src[i][j] > img_threshold ? 255 : 0;
            AT_IMAGE(dst, j, i, width) = AT_IMAGE(src, j, i, width);
}

void imgproc_ctrl::erode3(uint8_t src[120][160], uint8_t dst[120][160])
{
    int min_value;
    for (int y = 1; y < 120 - 1; y++)
    {
        for (int x = 1; x < 160 - 1; x++)
        {
            min_value = 255;
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (src[y + dy][x + dx] < min_value)
                        min_value = src[y + dy][x + dx];
                }
            }
            dst[y][x] = min_value;
        }
    }
}

void imgproc_ctrl::dilate3(uint8_t src[120][160], uint8_t dst[120][160])
{
    int max_value;
    for (int y = 1; y < 120 - 1; y++)
    {
        for (int x = 1; x < 160 - 1; x++)
        {
            max_value = 0;
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (src[y + dy][x + dx] > max_value)
                        max_value = src[y + dy][x + dx];
                }
            }
            dst[y][x] = max_value;
        }
    }
}

void imgproc_ctrl::gray_to_binary(uint8_t **src, uint8_t **dst, int height, int width, uint8_t mode)
{
    if (0 == mode)
        // img_threshold = mean_threshold(src);
        // img_threshold = (int)(img_threshold * 0.5) + 70;
        img_threshold = ostu_threshold(src, height, width);
    else if (1 == mode)
        img_threshold = ostu_threshold(src, height, width);
    else if (2 == mode)
        img_threshold = fix_threshold;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            // dst[i][j] = src[i][j] > img_threshold ? 255 : 0;
            AT_IMAGE(dst, j, i, width) = AT_IMAGE(src, j, i, width) > img_threshold ? 255 : 0;
}

int imgproc_ctrl::ostu_threshold(uint8 **img, int height, int width)
{
    for (int j = 0; j < 256; j++)
        HistoGram[j] = 0;

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            // HistoGram[img[j][i]]++;
            HistoGram[AT_IMAGE(img, i, j, width)]++;

    return solve_ostu();
}

void imgproc_ctrl::img_resize(uint8_t **src, uint8_t **dst, int src_height, int src_width, int scale)
{
    for (int i = 0; i < src_height / scale; i++)
        for (int j = 0; j < src_width / scale; j++)
            // dst[i][j] = src[scale * i][scale * j];
            AT_IMAGE(dst, j, i, src_width / scale) = AT_IMAGE(src, scale * j, scale * i, src_width);
}

void imgproc_ctrl::gray_to_binary(uint8_t src[120][160], uint8_t dst[120][160], uint8_t mode)
{
    if (0 == mode)
        img_threshold = fix_threshold;
    else if (1 == mode) //TODO
        img_threshold = ostu_threshold(src) - 4;
    else if (2 == mode)
        img_threshold = fix_threshold;
    for (int i = 0; i < 120; i++)
        for (int j = 0; j < 160; j++)
            dst[i][j] = src[i][j] > img_threshold ? 255 : 0;
}

int imgproc_ctrl::ostu_threshold(const uint8_t img[120][160])
{
    for (int j = 0; j < 256; j++)
        HistoGram[j] = 0;

    for (int j = 0; j < 160; j++)
        for (int i = 0; i < 120; i++)
            HistoGram[img[j][i]]++;

    return solve_ostu();
}

void imgproc_ctrl::img_resize(uint8_t src[240][320],
                              uint8_t dst[120][160])
{
    for (int i = 0; i < 120; i++)
        for (int j = 0; j < 160; j++)
            dst[i][j] = src[2 * i][2 * j];
}

int imgproc_ctrl::solve_ostu()
{
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ; //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue; // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue; // 图像中只有二个颜色

    for (int j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j]; //  像素总数

    PixelIntegral = 0;
    for (int j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }
    SigmaB = -1;
    for (int j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
        PixelFore = Amount - PixelBack;                                                    //背景像素点数
        OmegaBack = (float)PixelBack / Amount;                                             //前景像素百分比
        OmegaFore = (float)PixelFore / Amount;                                             //背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;                                  //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;                                  //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
        if (Sigma > SigmaB)                                                                //遍历最大的类间方差g
        //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;
}
