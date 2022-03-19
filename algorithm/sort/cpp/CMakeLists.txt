cmake_minimum_required(VERSION 3.5)

project(algorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(algorithm main.cpp QuickSort.cpp Sort.h SortTools.h PartFind.cpp
    BubbleSort.cpp)
