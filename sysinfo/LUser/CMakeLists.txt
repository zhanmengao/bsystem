cmake_minimum_required(VERSION 3.5)

project(LUser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LUser main.cpp UserManager.cpp UserManager.h)
