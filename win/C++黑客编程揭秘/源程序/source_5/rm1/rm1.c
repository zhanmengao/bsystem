#include <stdio.h>
#include <windows.h>

int test(char *szStr, int nNum)
{
    printf("%s, %d \r\n", szStr, nNum);
    MessageBox(NULL, szStr, NULL, MB_OK);
    
    return 5;
}

int main(int argc, char ** argv)
{
    int nNum = test("hello", 6);
    
    printf("%d \r\n", nNum);
    
    __asm push ebx

    return 0;
}