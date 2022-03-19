cmake_minimum_required(VERSION 3.5)

project(OSProcess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OSProcess LMain.cpp ChildProcess.cpp ChildProcess.h single.c OnceUtils.h)
