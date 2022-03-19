cmake_minimum_required(VERSION 3.10)
project(UnixTIme C)

set(CMAKE_C_STANDARD 99)

add_executable(UnixTIme main.c
        UnixTime.h
        Time.hpp)
