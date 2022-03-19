cmake_minimum_required(VERSION 3.17)
project(lfileinotify)

set(CMAKE_CXX_STANDARD 14)

add_executable(lfileinotify main.cpp FileWatch.h DirWatch.h)