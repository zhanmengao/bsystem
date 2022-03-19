cmake_minimum_required(VERSION 3.10)
project(LFileManager)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY"$  ~/ bin")
include_directories()
add_executable(LFileManager
        main.cpp
        FIleManager.h
        FIleManager.cpp
        PathHelper.h
        ../CFileManager.h FileAttrHelper.h)