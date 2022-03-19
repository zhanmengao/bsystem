cmake_minimum_required(VERSION 3.5)

project(mmapWrite LANGUAGES C)

add_executable(mmapWrite
    mmap_write.c)

target_link_libraries(mmapWrite)
