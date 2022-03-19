//
// Created by zhanmengao on 10/9/20.
//

#ifndef LERROR_ERROR_H
#define LERROR_ERROR_H
#include<stdio.h>
#include<string.h>
#include <errno.h>

#ifdef __linux__
void PError(const char* desc)
{
    perror(desc);
}
#endif
int Error()
{
    return errno;
}
const char* ErrorMessage(int error)
{
    return strerror(error);
}

#endif //LERROR_ERROR_H
