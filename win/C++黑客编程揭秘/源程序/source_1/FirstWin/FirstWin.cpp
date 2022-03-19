#include <windows.h>

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, 
               UINT uMsg, 
               WPARAM wParam,
               LPARAM lParam);

#define CLASSNAME "MyFirstWin"

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG  Msg;
    BOOL bRet;

    // 注册窗口类
    MyRegisterClass(hInstance);

    // 创建窗口并显示窗口
    if ( !InitInstance(hInstance, SW_SHOWNORMAL) )
    {
        return FALSE;
    }

    // 消息循环
    // 获取属于自己的消息并进行分发
    while( (bRet = GetMessage(&Msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            // handle the error and possibly exit
            break;
        }
        else
        {
            TranslateMessage(&Msg); 
            DispatchMessage(&Msg); 
        }
    }

    return Msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX WndCls;
    
    // 填充结构体为0
    ZeroMemory(&WndCls, sizeof(WNDCLASSEX));

    // cbSize是结构体大小
    WndCls.cbSize = sizeof(WNDCLASSEX);
    // lpfnWndProc是窗口过程地址
    WndCls.lpfnWndProc = WindowProc;
    // hInstance是实例句柄
    WndCls.hInstance = hInstance;
    // lpszClassName是窗口类类名
    WndCls.lpszClassName = CLASSNAME;
    // style是窗口类风格
    WndCls.style = CS_HREDRAW | CS_VREDRAW;
    // hbrBackground是窗口类背景色
    WndCls.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME + 1;
    // hCursor是鼠标句柄
    WndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
    // hIcon是图标句柄
    WndCls.hIcon = LoadIcon(NULL, IDI_QUESTION);
    // 其他
    WndCls.cbClsExtra = 0;
    WndCls.cbWndExtra = 0;
    
    return RegisterClassEx(&WndCls);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = NULL;

    // 创建窗口
    hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                    CLASSNAME,
                    "MyFirstWindow",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, 
                    NULL, NULL, hInstance, NULL);

    if ( NULL == hWnd )
    {
        return FALSE;
    }

    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    // 更新窗口
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WindowProc(
                HWND hwnd, 
                UINT uMsg, 
                WPARAM wParam,
                LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC;
    RECT rt;

    char *pszDrawText = "Hello Windows Program.";

    switch (uMsg)
    {
    case WM_PAINT:
        {
            hDC = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rt);
            DrawTextA(hDC, 
                    pszDrawText, strlen(pszDrawText),&rt, 
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hwnd, &ps);
    	    break;
        }
    case WM_CLOSE:
        {
            if ( IDYES == MessageBox(hwnd, 
                  "是否退出程序", "MyFirstWin", MB_YESNO) )
            {
                DestroyWindow(hwnd);
                PostQuitMessage(0);
            }
            break;
        }
    default:
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}