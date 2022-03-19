#pragma once
#include <windows.h>  
#include <process.h>  
#include <stdio.h>  
#include <stdlib.h>

void mainCreate();

int WaitForSingleObject0();
int WaitForSingleObject5000();
int WaitForSingleObjectINFINITE();

int WaitForMultipleObjectsFalse();
int WaitForMultipleObjectsTrue();

int mainCriticalSection1();
int mainCriticalSection2();

int mainMutex();
int mainSemaphore();
int mainEvent();
int mainrwLock();
void FileLockMain();

