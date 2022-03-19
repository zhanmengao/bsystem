cmake_minimum_required(VERSION 3.5)

project(sds LANGUAGES C)

add_executable(sds main.c sds.h sds.c zmalloc.h zmalloc.c adlist.h adlist.c)
