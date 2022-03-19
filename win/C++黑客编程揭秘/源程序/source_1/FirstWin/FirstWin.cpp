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

    // ע�ᴰ����
    MyRegisterClass(hInstance);

    // �������ڲ���ʾ����
    if ( !InitInstance(hInstance, SW_SHOWNORMAL) )
    {
        return FALSE;
    }

    // ��Ϣѭ��
    // ��ȡ�����Լ�����Ϣ�����зַ�
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
    
    // ���ṹ��Ϊ0
    ZeroMemory(&WndCls, sizeof(WNDCLASSEX));

    // cbSize�ǽṹ���С
    WndCls.cbSize = sizeof(WNDCLASSEX);
    // lpfnWndProc�Ǵ��ڹ��̵�ַ
    WndCls.lpfnWndProc = WindowProc;
    // hInstance��ʵ�����
    WndCls.hInstance = hInstance;
    // lpszClassName�Ǵ���������
    WndCls.lpszClassName = CLASSNAME;
    // style�Ǵ�������
    WndCls.style = CS_HREDRAW | CS_VREDRAW;
    // hbrBackground�Ǵ����౳��ɫ
    WndCls.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME + 1;
    // hCursor�������
    WndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
    // hIcon��ͼ����
    WndCls.hIcon = LoadIcon(NULL, IDI_QUESTION);
    // ����
    WndCls.cbClsExtra = 0;
    WndCls.cbWndExtra = 0;
    
    return RegisterClassEx(&WndCls);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = NULL;

    // ��������
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

    // ��ʾ����
    ShowWindow(hWnd, nCmdShow);
    // ���´���
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
                  "�Ƿ��˳�����", "MyFirstWin", MB_YESNO) )
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