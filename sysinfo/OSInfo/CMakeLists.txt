cmake_minimum_required(VERSION 3.5)
project(OSInfo)

set(CMAKE_CXX_STANDARD 14)

add_executable(OSInfo
        main.cpp
        NetInfo.h CPUInfo.h OSInfo.h MemInfo.h)
