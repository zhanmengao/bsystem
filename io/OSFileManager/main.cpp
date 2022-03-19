#include"CFileUtils.h"
#include "OSFile.h"
#include<iostream>
#ifdef __linux
int main()
{
	FILE* pfile=FOpen("main.cpp","r+");
	if(!pfile)
    {
        printf("not search main.cpp \n");
    }
	else
    {
        printf("fd = %d size = %lu \n",Getfd(pfile),FSize(pfile));
    }
	return 0;
}
#else
void main()
{
	OSFile file;
	file.Open(_T("main.cpp"));
	PSID sid = file.GetOwner();
	auto ps = file.GetSecurityDesc();
	std::cout << file.Size() << std::endl;
}
#endif
