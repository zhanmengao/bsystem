cmake_minimum_required(VERSION 3.10)
project(PthreadPool C)

set(CMAKE_C_STANDARD 99)
#添加链接库
#TARGET_LINK_LIBRARIES(PthreadPool pthread.a)

#add_executable(PthreadPool main.c)
add_executable(PthreadPool threadpool.h threadpool.c)

target_link_libraries(PthreadPool pthread)  #link
