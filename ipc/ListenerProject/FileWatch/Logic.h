#pragma  once

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <iomanip>
using namespace std;

void ThreadUser();
void ThreadFifo();

void fileWatcher();
void StartThread();


bool CheckFile(const wstring& filePath);
bool IsModNeedSave(const string& filePath);

