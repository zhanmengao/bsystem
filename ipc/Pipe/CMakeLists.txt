cmake_minimum_required(VERSION 3.10)
project(PipeHelper)

set(CMAKE_CXX_STANDARD 14)

add_executable(PipeHelper Lmain.cpp PipeHelper.h PipeHelper.cpp
    ../../Process/WindowsProcessHelper/Fork/ChildProcess.cpp
    ../../Process/WindowsProcessHelper/Fork/ChildProcess.h
    AbstractIPC.h)
