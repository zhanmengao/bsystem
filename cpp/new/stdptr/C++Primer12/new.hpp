#pragma once
#include<new>

void mainNew()
{
	const int *pci = new const int(1024);				//new����const����Ϸ�

#ifdef _WIN32
	int *p2 = new (std::nothrow) int;			//��λnew�������ڴ�ľ���ʱ�����쳣
    delete p2;
    p2 = nullptr;
    delete p2;				//delete��ָ�룬û����
#endif
	delete pci;

	int i1;
	//delete &i1;				//����delete��ջ�ڴ�

}
