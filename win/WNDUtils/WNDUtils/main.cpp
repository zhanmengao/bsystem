#include<Windows.h>
#include"WindowHelper.h"
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return (DefWindowProc(hwnd, message, wParam, lParam));
};
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	WindowHelper window;
	window.InitWindow(WindowProc,hInstance,0,0);
	//window.Show(nCmdShow);
	MSG msg = { 0 };
	while (1)
	{
		window.Update();
		Sleep(100);
	}
	return 0;
}