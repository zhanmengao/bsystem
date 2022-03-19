cmake_minimum_required(VERSION 3.5)

project(CpFile LANGUAGES C)

add_executable(CpFile
    #BigFileCpy_file.c
    BigFileCpy_thread.c)

target_link_libraries(CpFile -lpthread)
