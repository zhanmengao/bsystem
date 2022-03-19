cmake_minimum_required(VERSION 3.5)

project(stdptr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
aux_source_directory(C++Primer12 SRCDIR)
include_directories(./C++Primer12)

FILE(GLOB hpplist ${PROJECT_SOURCE_DIR}/C++Primer12/*.hpp)
FILE(GLOB hlist ${PROJECT_SOURCE_DIR}/C++Primer12/*.h)

add_executable(stdptr ${SRCDIR} ${hpplist} ${hlist})
