cmake_minimum_required(VERSION 3.19)
project(car)

set(CMAKE_CXX_STANDARD 14)

add_executable(car
        main.cpp
        common.h
        common_macro.h
        f_elems.cpp
        f_cross.cpp
        f_curve.cpp
        f_garage.cpp
        f_loop.cpp
        f_qrcode.cpp
        f_ramp.cpp
        f_yroad.cpp
        util.cpp
#        ../Project/src/find_lines.cpp
        f_elems.h
        img_proc.cpp
        main.h)