#include <iostream.h>

int main(int argc, char* argv[])
{
    // 生命三个整型的指针变量
    int *p = NULL;      // 32位的整型变量指针
    __int64 *q = NULL;  // 64位的整型变量指针
    int *m = NULL;      // 32位的整型变量指针
    
    // 使用new分配一个整型的内存空间
    // 用指针变量p指向该内存空间
    p = new int;
    if ( p == NULL )
    {
        return -1;
    }
    
    // 为指针变量p指向的内存空间赋值
    *p = 0x11223344;


    // q和m操作同p
    q = new __int64;
    if ( q == NULL )
    {
        return -1;
    }
    
    *q = 0x1122334455667788;

    m = new int;
    if ( m == NULL )
    {
        return -1;
    }
    
    *m = 0x11223344;
 
    // 释放三个变量指向的地址空间
    // 释放顺序依次是q、m、p
    delete q;
    q = NULL;
    
    delete m;
    m = NULL;
    
    delete p;
    p = NULL;
    
    return 0;
}