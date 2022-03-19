cmake_minimum_required(VERSION 3.10)
project(FileUtils)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(FileUtils
        CFileUtils.h
        CppFileUtils.hpp
        main.cpp
        OSUtils.h
        OSFile.h
        OSFile.cpp
        OSFileManager.h
        OSFileManager.cpp)
