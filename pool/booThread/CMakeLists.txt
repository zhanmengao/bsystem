cmake_minimum_required(VERSION 3.5)

project(booThread LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. DIR)
add_executable(booThread main.cpp join.cpp detach.cpp  Move.cpp  thread.cpp Define.h
    booThread.h booWait.h booCancel.h booCancel2.h booAttr.h)

target_link_libraries(booThread -lpthread -lboost_thread -lboost_timer -lboost_chrono)
