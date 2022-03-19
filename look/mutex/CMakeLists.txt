cmake_minimum_required(VERSION 3.5)

project(mutex_rwlock)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mutex_rwlock
    #muetx.c
    deadlock.c
    #rwlock.c
    )

target_link_libraries(mutex_rwlock -lpthread)
