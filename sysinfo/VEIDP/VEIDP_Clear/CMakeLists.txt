cmake_minimum_required(VERSION 3.5)

project(IDPClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(IDPClient main.cpp
    UserRoutinue.h UserRoutinue.cpp UserMessage.h UserMessage.cpp
    Config.cpp Config.h
    CollectMessage.h CollectMessage.cpp CollectRoutinue.h CollectRoutinue.cpp LCollectLogic.cpp
    ../../../Process/ThreadPool/ThreadPool/ThreadBase.h
    ../../../Process/ThreadPool/ThreadPool/ThreadBase.cpp
    ../../../File/FileManager/SocketCtrl/SocketCtrlCommon/LogThread.cpp
    ../../../Process/ThreadPool/ThreadPool/ThreadManager.h
    ../../../Process/ThreadPool/ThreadPool/ThreadManager.cpp
    ../../../Process/ThreadPool/ThreadPool/PthreadPool_BlockQueue.cpp
    ../../../Process/ThreadPool/ThreadPool/PthreadPool_Manager.cpp
    ../../../Process/ThreadPool/ThreadPool/PthreadPool.cpp
    ../../../../algorithm/Normal/byd/stb/CString.h ../../../../algorithm/Normal/byd/stb/CString.cpp
    ../../../File/FileManager/LFileManager/FIleManager.h
    ../../../File/FileManager/LFileManager/FIleManager.cpp)

target_link_libraries(IDPClient -ljsoncpp -lpthread)
