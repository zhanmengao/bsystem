#pragma once
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>
#include <Winsvc.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

class CEnablePriv
{
public:            //����(���⿪�ŵĽӿ�)

				   //
				   //���õ�ǰ�������ȼ�Ϊ���(ʵʱ)
				   //
	static bool SetRealTimePriority();

	//
	//������ǰ����Ȩ�޺���(��SeShutdownPrivilege���ػ�Ȩ��)
	//
	static bool EnableShutdownPriv();

	//
	//������ǰ����Ȩ�޺���(��SeDebugPrivilege������д����Ȩ��)
	//
	static bool EnableDebugPriv();

	//
	//������ǰ����Ȩ�޺���(��SeBackupPrivilege��ע�����Ȩ��)
	//
	static bool EnableBackupPriv();

	//
	//������ǰ����Ȩ�޺���(��SeRestorePrivilege���ָ�����Ȩ��)
	//
	static bool EnableRestorePriv();

private:           //˽��(�ڲ�ʹ�õĽӿ�)

};
