cmake_minimum_required(VERSION 3.10)
project(sem)

set(CMAKE_CXX_STANDARD 14)


add_executable(sem sem_c_p.cpp)

target_link_libraries(sem -lpthread)
