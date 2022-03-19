#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hFile = CreateFile("c:\\test.txt", GENERIC_READ, 
                    FILE_SHARE_READ, NULL, OPEN_EXISTING, 
                    FILE_ATTRIBUTE_NORMAL, NULL);

    if ( hFile == INVALID_HANDLE_VALUE )
    {
        printf("Err Code = %d \r\n", GetLastError());
    }

    return 0;
}