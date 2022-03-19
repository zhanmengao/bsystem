cmake_minimum_required(VERSION 3.10)
project(CFileIO)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CFileIO
        dm01_按照字符读写文件.c
        dm02_按照行读写文件.c
        dm03_按照块的方式操作文件.c
)
