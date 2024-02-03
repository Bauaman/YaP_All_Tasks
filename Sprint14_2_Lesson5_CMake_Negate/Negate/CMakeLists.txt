cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(Negate CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(negate src/main.cpp src/img_lib.h src/img_lib.cpp src/ppm_image.h src/ppm_image.cpp)
target_link_libraries(negate -lstdc++)