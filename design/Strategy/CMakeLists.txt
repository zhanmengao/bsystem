cmake_minimum_required(VERSION 3.5)

project(Strategy LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Strategy main.cpp Strategy.h Strategy.cpp)

find_package(Qt5Core)
find_package(Qt5Widgets)

TARGET_LINK_LIBRARIES( Strategy Qt5::Widgets)
