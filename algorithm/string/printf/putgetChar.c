#include"SysDefine.h"


int mainGetchar()
{
	int c;
	while ((c = getchar() )!= EOF)
	{
		putchar(tolower(c));
		if(c=='q')
		{
			return 1;
		}
	}
	return 22;
}