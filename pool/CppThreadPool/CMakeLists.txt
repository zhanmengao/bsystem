cmake_minimum_required(VERSION 3.10)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

aux_source_directory(. SRCS)

add_executable(ThreadPool ${SRCS}
    PP.hpp PthreadPool.h)
target_link_libraries(ThreadPool pthread)  #link
