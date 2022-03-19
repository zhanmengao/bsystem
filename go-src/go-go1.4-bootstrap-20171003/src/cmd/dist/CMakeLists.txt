cmake_minimum_required(VERSION 3.10)

project(cmd C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

aux_source_directory(. src)
add_executable(cmd ${src})
