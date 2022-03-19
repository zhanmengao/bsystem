cmake_minimum_required(VERSION 3.10)
project(cond)

set(CMAKE_CXX_STANDARD 14)


add_executable(cond
    #cond_c_p.cpp
    cond_timedwait.cpp
    condvar.c
    )

target_link_libraries(cond -lpthread)
