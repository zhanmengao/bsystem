#include <Windows.h>
#include <stdio.h>

int main333(int argc, char* argv[])
{
	//当前屏幕的分辨率
	int nScreenWidth, nScreenHeight;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	printf("当前屏幕的分辨率为：%d×%d \n", nScreenWidth, nScreenHeight);


	//当前屏幕数量
	int screenNum;
	screenNum = GetSystemMetrics(SM_CMONITORS);
	printf("当前屏幕数量：%d\n", screenNum);

	//当前屏幕总的分辨率
	int aScreenWidth, aScreenHeight;
	aScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	aScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	printf("当前屏幕总的分辨率为：%d×%d \n", aScreenWidth, aScreenHeight);

	return 0;
}