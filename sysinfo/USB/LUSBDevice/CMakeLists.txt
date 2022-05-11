cmake_minimum_required(VERSION 3.10)
project(LUSBDevice)

set(CMAKE_CXX_STANDARD 14)

add_executable(LUSBDevice main.cpp USBUtils.h USBUtils.cpp status.cpp Transfer.c mainInfo.cpp lsusb.h mainList.cpp xusb.h)

target_link_libraries(LUSBDevice -lusb-1.0)
