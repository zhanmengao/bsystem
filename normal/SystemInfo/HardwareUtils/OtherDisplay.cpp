#include <Windows.h>
#include <stdio.h>

int main333(int argc, char* argv[])
{
	//��ǰ��Ļ�ķֱ���
	int nScreenWidth, nScreenHeight;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	printf("��ǰ��Ļ�ķֱ���Ϊ��%d��%d \n", nScreenWidth, nScreenHeight);


	//��ǰ��Ļ����
	int screenNum;
	screenNum = GetSystemMetrics(SM_CMONITORS);
	printf("��ǰ��Ļ������%d\n", screenNum);

	//��ǰ��Ļ�ܵķֱ���
	int aScreenWidth, aScreenHeight;
	aScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	aScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	printf("��ǰ��Ļ�ܵķֱ���Ϊ��%d��%d \n", aScreenWidth, aScreenHeight);

	return 0;
}