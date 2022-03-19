cmake_minimum_required(VERSION 3.10)
project(FileIO)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(FileIO
        BinaryFileRW.hpp
        Other2.h
        sstream.cpp
        Stream.cpp
        WFile.cpp
        文件IO.cpp
        文件的读写.cpp
        标准输入test.cpp
        标准输入流.cpp
        标准输出.cpp
        输出格式.cpp)
