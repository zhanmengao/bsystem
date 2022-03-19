cmake_minimum_required(VERSION 3.10)
project(Byte)

set(CMAKE_CXX_STANDARD 14)

add_executable(Byte main.cpp BitUtils.cpp BitUtils.h ByteUtils.h)
