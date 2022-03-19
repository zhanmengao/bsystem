cmake_minimum_required(VERSION 3.5)

project(sigqueue)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sigqueue sigset_sigprocmask_sigpending.c)
