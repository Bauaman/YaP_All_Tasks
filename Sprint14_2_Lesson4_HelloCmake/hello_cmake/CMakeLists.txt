cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(HelloCmake CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(hello src/main.cpp src/hello_printer.cpp src/hello_printer.h)
target_link_libraries(hello -lstdc++)