cmake_minimum_required(VERSION 3.16)
project(libqueue C)

set(CMAKE_C_STANDARD 99)
aux_source_directory(. DIR_SRCS)
# Add header file include directories
include_directories(.)

#设置生成可执行文件
add_executable(libqueue ${DIR_SRCS} queue.h)
