#include <iostream.h>

int main(int argc, char* argv[])
{
    // �����������͵�ָ�����
    int *p = NULL;      // 32λ�����ͱ���ָ��
    __int64 *q = NULL;  // 64λ�����ͱ���ָ��
    int *m = NULL;      // 32λ�����ͱ���ָ��
    
    // ʹ��new����һ�����͵��ڴ�ռ�
    // ��ָ�����pָ����ڴ�ռ�
    p = new int;
    if ( p == NULL )
    {
        return -1;
    }
    
    // Ϊָ�����pָ����ڴ�ռ丳ֵ
    *p = 0x11223344;


    // q��m����ͬp
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
 
    // �ͷ���������ָ��ĵ�ַ�ռ�
    // �ͷ�˳��������q��m��p
    delete q;
    q = NULL;
    
    delete m;
    m = NULL;
    
    delete p;
    p = NULL;
    
    return 0;
}