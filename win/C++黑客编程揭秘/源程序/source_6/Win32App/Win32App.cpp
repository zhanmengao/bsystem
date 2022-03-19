#include <Windows.h>

int WINAPI WinMain(IN HINSTANCE hInstance, 
                   IN HINSTANCE hPrevInstance, 
                   IN LPSTR lpCmdLine, 
                   IN int nShowCmd)
{
    MessageBox(NULL, "hello world!", "hello", MB_OK);

    return 0;
}
