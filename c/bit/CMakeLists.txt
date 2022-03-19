cmake_minimum_required(VERSION 3.10)
project(bit)

set(CMAKE_CXX_STANDARD 14)


add_executable(bit
    BitUtils.h
    BitUtils.cpp
    main.cpp
    )
    
add_executable(Byte
    ByteUtils.h
    byte.cpp
    BitUtils.h
    BitUtils.cpp
    )
