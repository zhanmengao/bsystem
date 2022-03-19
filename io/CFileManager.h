#pragma once
#ifdef _WIN32
#include<io.h>
#define F_OK 0
#define X_OK 6
#define W_OK 4
#define R_OK 2
#else
#include <unistd.h>
#endif

static bool IsFileExist(const char* filePath)
{
	return access(filePath, F_OK) == 0;
}
static bool IsFileCanRead(const char* filePath)
{
	return access(filePath, R_OK) == 0;
}
static bool IsFileCanWrite(const char* filePath)
{
	return access(filePath, W_OK) == 0;
}
static bool IsFileCanExec(const char* filePath)
{
	return access(filePath, X_OK) == 0;
}