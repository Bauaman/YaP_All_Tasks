cmake_minimum_required(VERSION 3.11)

project(HelloCMake CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(ppm_image main.cpp img_lib.h img_lib.cpp ppm_image.h ppm_image.cpp)