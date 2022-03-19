#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<fcntl.h>
#include <string.h>
#include <stdarg.h>
#include<fcntl.h>
#include <ctype.h>
#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#define STDIN_FILENO 0
#ifdef _WIN64
//define something for Windows (64-bit only)
#else
//define something for Windows (32-bit only)
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif
#elif __ANDROID__
// android
#elif __linux__
// linux
#include <unistd.h>
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#   error "Unknown compiler"
#endif