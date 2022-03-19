cmake_minimum_required(VERSION 3.10)
project(ThreadHelper)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ThreadHelper
        ThreadUtils.h
        main.cpp
        ThreadUtils.cpp
        OSThread.cpp
        OSThread.h
        CppThread.cpp
        CppThread.h
        )
target_link_libraries(ThreadHelper pthread)  #link
