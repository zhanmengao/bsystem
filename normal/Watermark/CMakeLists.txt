cmake_minimum_required(VERSION 3.10)
project(Watermark)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

#Qt PrefixPath
set(CMAKE_PREFIX_PATH /home/uzi/Qt5.12.9/5.12.9/gcc_64)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_executable(Watermark main.cpp Watermark.cpp Watermark.h)




#qt5_wrap_ui( ui_FILES mainwindow.ui)
#qt5_add_resources( QRC_FILES GetCloudAppOpenFile.qrc)
#add_executable(CloudAppOpen ${QRC_FILES} ${SOURCE_FILES})

# Link Qt library
TARGET_LINK_LIBRARIES(Watermark Qt5::Widgets Qt5::Network)
