#include "cstdio"
#include "cmath"
#include <string>
#include "f_elems.h"
#include "img_proc.h"
using namespace std;
#define USE_GRAY 0
#define FILE_LEN 3

chassis_ctrl chassis;
inoutdev_ctrl inoutdev;
imgproc_ctrl img_proc_c;

void inoutdev_ctrl::beep(int num) { printf("beep.\n"); }
void inoutdev_ctrl::lbeep(int num) { printf("lbeep.\n"); }
int inoutdev_ctrl::switch_is_on(int num) { return 1; };
int read_txt_240(const char *file);
int read_txt_120(const char *file);
void save_txt_120(const char *file);
void save_txt_240(const char *file);
void draw_midline();
void chassis_bake() {}

char *readPath[FILE_LEN] = {
    //        ".\\src_txt_120\\54.txt",
    //        ".\\src_txt_120\\35.txt",
    //        ".\\src_txt_120\\25.txt",
    //        ".\\src_txt_120\\8.txt",
    //        ".\\src_txt_120\\11.txt",
    //        ".\\src_txt_120\\25.txt",

    //        ".\\src_txt_240\\1.txt",
    //        ".\\src_txt_240\\11.txt",

//    ".\\src_txt_240\\cross1.txt",
//    ".\\src_txt_240\\cross2.txt",

    //        ".\\src_txt_240\\fork1.txt",
    //        ".\\src_txt_240\\fork2.txt",
//    ".\\src_txt_240\\fork3.txt",

                ".\\src_txt_120\\23.txt",
                ".\\src_txt_120\\23.txt",
                ".\\src_txt_120\\22.txt",
};


int main(int argc, char *argv[])
{
    for (int i = 0; i < FILE_LEN; i++)
    {
#if USE_GRAY
        string file_name = readPath[i];
        if (read_txt_240(file_name.data()) != 1)
            continue;

//        img_proc_c.img_resize(gray_240_320_image, gray_120_160_image);
//        img_proc_c.fix_threshold = img_proc_c.ostu_threshold(gray_120_160_image);
//        img_proc_c.gray_to_binary(gray_240_320_image, bin_240_320_image, 2);
//        img_proc_c.img_resize(bin_240_320_image, bin_120_160_image);
        img_proc_c.img_resize((uint8 **)gray_240_320_image, (uint8 **)gray_120_160_image, 240, 320, 2);
        img_proc_c.gray_to_binary((uint8 **)gray_120_160_image, (uint8 **)bin_120_160_image, 120, 160, 1);

#else
        string file_name = readPath[i];
        if (read_txt_120(file_name.data()) != 1)
            continue;
            //img_proc_c.imopen2();

#endif
        find_track_gray();
        find_elements();
        //        find_qrcode_pos();

        draw_midline();

        string save_name = file_name.replace(file_name.find("s"), 11, "dst_txt_120");
        save_txt_120(save_name.data());
        //        string save_name2=file_name.replace(file_name.find("s"),11,"dst_txt_240");
        //        save_txt_240(save_name2.data());
    }
    return 0;
}
