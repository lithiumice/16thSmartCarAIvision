import cv2
import numpy as np
import glob


def txt2img(file):
    img = []
    number_list = [str(i) for i in range(10)]

    with open(file, 'r') as f:
        times = 0
        for list__ in f.readlines():
            hundred = 0
            ten = 0
            cache = []
            for i in list__[:-1]:
                times += 1
                if times == 1:
                    hundred = 100 * int(i) if str(i) in number_list else 0
                if times == 2:
                    ten = 10 * int(i) if str(i) in number_list else 0
                if times == 3:
                    times = 0
                    num = hundred + ten + int(i)
                    hundred = 0
                    ten = 0
                    cache.append(num)
            img.append(cache)
    import numpy as np
    img = np.array(img)
    return img


def bmp2txt_120(bmp_file):
    file_name = bmp_file.split('\\')[-1].split('.')[0]
    file_name = './src_txt_120/%s.txt' % file_name
    img = cv2.imread(bmp_file, cv2.IMREAD_GRAYSCALE)
    np.savetxt(file_name, img, "%3d")


def bmp2txt_240(bmp_file):
    file_name = bmp_file.split('\\')[-1].split('.')[0]
    file_name = './src_txt_240/%s.txt' % file_name
    img = cv2.imread(bmp_file, cv2.IMREAD_GRAYSCALE)
    np.savetxt(file_name, img, "%3d")


def txt2bmp(txt_file):
    # file_name = txt_file.split('\\')[-1].split('.')[0]
    # file_name = 'dst_bmp/%s.bmp' % file_name
    file_name = txt_file.split('.')
    file_name[-1] = 'bmp'
    file_name = '.'.join(file_name)
    img_ = txt2img(txt_file)
    cv2.imwrite(file_name, img_, )


def bmp2txt_patch_120():
    # 把bmp保存为txt
    files = glob.glob('src_txt_120/*.bmp')
    for file in files:
        bmp2txt_120(file)


def bmp2txt_patch_240():
    # 把bmp保存为txt
    files = glob.glob('src_txt_240/*.bmp')
    for file in files:
        bmp2txt_240(file)


def txt2bmp_patch_240():
    # 把txt还原为bmp
    files = glob.glob('dst_txt_240/*.txt')
    for file in files:
        print(file)
        txt2bmp(file)


def txt2bmp_patch_120():
    # 把txt还原为bmp
    files = glob.glob('dst_txt_120/*.txt')
    for file in files:
        print(file)
        txt2bmp(file)


if __name__ == '__main__':
    # txt2bmp_patch_120()
    txt2bmp_patch_240()
    # bmp2txt_patch_120()
