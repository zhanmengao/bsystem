cmake_minimum_required(VERSION 3.5)

project(CallCmd LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)


add_executable(CallCmd
  main.cpp /
../../../../Network/RDP/UAMLinux/GetCloudAppOpenFile2020/Strategy.h /
../../../../Network/RDP/UAMLinux/GetCloudAppOpenFile2020/Strategy.cpp
)
target_link_libraries(CallCmd Qt5::Core Qt5::Widgets Qt5::Network crypto)
