cmake_minimum_required(VERSION 3.5)

project(Sem LANGUAGES C)

add_executable(Sem main.c)

target_link_libraries(Sem -lpthread)
