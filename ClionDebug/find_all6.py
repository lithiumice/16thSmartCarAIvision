import pyb
import time
import sensor
import image
import nncu
from machine import UART
from pyb import LED
from machine import Pin

red = LED(1)
green = LED(2)
blue = LED(3)
white = LED(4)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_brightness(500)
sensor.skip_frames(time=20)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

net_path_num = "model_number2.nncu"
net_path_anifruit = "model_anifruit2.nncu"
net_num_labels = [i for i in range(0, 10)]
net_num = nncu.load(net_path_num, load_to_fb=True)
net_anifriut = nncu.load(net_path_anifruit, load_to_fb=True)

uart = UART(1, 115200)
pin10 = Pin(("B0", 0))
pin10.init(Pin.IN, Pin.PULL_UP)

regco_flag = 3  # 0：识别二维码 1：识别数字 2：识别动物水果 3: 打靶
regco_flag_reg = -1
err_cnt = 0
led_sm = 0
l_time = 0
blob_sm = 0
min_x_error = 1000


def write_uart(regco_flag, classes, conf):
    try:
        uart.write("{},{},{}\n".format(
            regco_flag, classes, conf))
    except:
        pass


def blink_white():
    white.on()
    pyb.mdelay(300)
    white.off()


while(True):
    if not pin10.value():
        blink_white()
        regco_flag = regco_flag+1
        if regco_flag > 3:
            regco_flag = 0
        while(not pin10.value()):
            pass

    l_time = l_time+1
    if l_time >= 3:
        l_time = 0
        if led_sm:
            green.on()
            led_sm = 0
        else:
            green.off()
            led_sm = 1

    print("regco_flag: {}".format(regco_flag))

    try:
        data = uart.read()
        if data:
            print("rev from ctrl: {}".format(data))
            r = int(data)
            if r >= 0 and r <= 3:
                blink_white()
                regco_flag_reg = regco_flag
                regco_flag = r
                uart.write("ok\n")
    except:
        pass

    img = sensor.snapshot()

    if regco_flag == 0:
        for tag in img.find_apriltags(roi=(60, 120, 200, 120), families=image.TAG25H9):
            img.draw_rectangle(tag.rect(), color=(255, 0, 0))
            if tag.id() > 9:
                continue
            num = tag.id()
            write_uart(regco_flag, num, 100)
            print("sended: {},{},{},{}".format(regco_flag, num, 100, 10))
    elif regco_flag == 1:
        for r in img.find_rects(threshold=40000, roi=(70, 45, 180, 140)):
            # for r in img.find_blobs([(19, 27, 9, 22, -25, -5)],invert = False, roi = (70,50,180,140), area_threshold = 600, pixels_threshold = 450):
            rad = r.h()/r.w()
            if rad > 0.7 and rad < 1.6:
                img1 = img.copy(r.rect())
                for obj in nncu.classify(net_num, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                    even_conf = obj.output()[0]
                    odd_conf = obj.output()[1]
                    if even_conf > odd_conf:
                        classes = 0
                        conf = even_conf*100
                    else:
                        classes = 1
                        conf = odd_conf*100
                    if conf > 85:
                        write_uart(regco_flag, classes, conf)
                        print("sended: {},{},{},{}".format(
                            regco_flag, classes, conf, 10))
    elif regco_flag == 2:
        for r in img.find_rects(threshold=40000, roi=(70, 45, 180, 140)):
            # for r in img.find_blobs([(26, 38, 11, 35, -45, -18)],invert = False, roi = (70,50,180,140), area_threshold = 600, pixels_threshold = 450):
            rad = r.h()/r.w()
            if rad > 0.7 and rad < 1.6:
                img1 = img.copy(r.rect())
                for obj in nncu.classify(net_anifriut, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                    ani_conf = obj.output()[0]
                    friut_conf = obj.output()[1]
                    if ani_conf > friut_conf:
                        classes = 0
                        conf = ani_conf*100
                    else:
                        classes = 1
                        conf = friut_conf*100
                    if conf > 85:
                        write_uart(regco_flag, classes, conf)
                        print("sended: {},{},{},{}".format(
                            regco_flag, classes, conf, 10))
    elif regco_flag == 3:  # 寻找合适的打靶位置
        #img = img.scale(x_scale=0.5, y_scale=0.5, load_to_fb=True)
        img.draw_rectangle(20, 45, 280, 140, color=(0, 255, 0))
        rects = img.find_rects((20, 45, 280, 140), threshold=40000)
        if rects:
            for r in rects:
                rad = r.h()/r.w()
                if rad > 0.7 and rad < 1.5:
                    img.draw_rectangle(r.rect(), color=(255, 0, 255))
                    pos_x = r.x()+r.w()/2
                    write_uart(regco_flag, (pos_x - img.width()/2), 100)
                    print("sneded: {},{},{},{}".format(
                        regco_flag, (pos_x - img.width()/2), 100, rad))
                    print(rad)
    elif regco_flag == 4:
        for r in img.find_rects((50, 45, 220, 140), threshold=40000):
            rad = r.h()/r.w()
            if rad > 0.9 and rad < 1.2:
                pos_x = r.x()+r.w()/2
                write_uart(regco_flag, (pos_x - img.width()/2), 100)
                print("sended: {},{},{},{}".format(
                    regco_flag, (pos_x - img.width()/2), 100, rad))
        #img.draw_cross(int(img.width()/2),int(img.height()/2),color = (255,0,255))
            #write_uart(regco_flag, (cx - img.width()/2), 100)
            #print("sended: {},{},{}".format(regco_flag, (cx - img.width()/2), 100))
        # for r in img.find_rects(threshold=30000, roi = (60,40,200,160)):
            #img.draw_rectangle(r.rect(),color = (255,0,255))
            #rad = r.h()/r.w()
            # if rad > 0.5 and rad < 2.0:
                #pos_x = r.x()+r.w()/2
                #pos_y = r.y()+r.h()/2
                #img.draw_cross(int(pos_x),int(pos_y),color = (255,0,0))
                #write_uart(regco_flag, (pos_x-img.width()/2), 100)
                # print("sended: {},{},{}".format(
                # regco_flag, (pos_x-img.width()/2), 100))
