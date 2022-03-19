cmake_minimum_required(VERSION 3.5)

project(mmtest LANGUAGES C)

add_executable(mmtest main.c)

add_executable(mmtest2 main2.c)

target_link_libraries(mmtest -lmm)
target_link_libraries(mmtest2 -lmm)
