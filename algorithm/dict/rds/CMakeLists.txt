cmake_minimum_required(VERSION 3.5)

project(redis)
include_directories(../../queue/cpp)
include_directories(../../list/Liner)
include_directories(../../sort/cpp)
add_executable(dict dictmain.c dict.h dict.c fmacros.h zmalloc.c zmalloc.h redisassert.h)

add_executable(ziplist fmacros.h zmalloc.c zmalloc.h redisassert.h ziplist.h ziplist.c
    util.c util.h zipmain.c config.h sds.c sds.h)
