#ifndef _RAW_INPUT_TEST_H_
#define _RAW_INPUT_TEST_H_


#include <stdio.h>
#include <Windows.h>


// ע��ԭʼ�����豸
BOOL Init(HWND hWnd);

// ��ȡԭʼ��������
BOOL GetData(LPARAM lParam);

// ���水����Ϣ
void SaveKey(USHORT usVKey);


#endif