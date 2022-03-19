#include"SysDefine.h"

void reads()
{
	char buffer[1024];
	int ret;
	if ((ret = read(STDIN_FILENO,buffer,1024) )> 0)
	{
		printf("s=%-5.10s\n", buffer);
		memset(buffer,0,ret);
	}
}
void reado()
{
	char buffer[1024];
	int ret;
	if ((ret = read(STDIN_FILENO,buffer,1024) )> 0)
	{
		printf("s=%s,o=%o\n", buffer,buffer);
		memset(buffer,0,ret);
	}
}

void readx()
{
	char buffer[1024];
	int ret;
	if ((ret = read(STDIN_FILENO,buffer,1024) )> 0)
	{
		printf("s=%s,x=%x\n", buffer,buffer);
		memset(buffer,0,ret);
	}
}
int mainFormat()
{

	char common;
	while(common=getchar())
	{
		if(common=='q')
	    {
	    	break;
	    } 
	    else if(common=='s')
	    {
	    	reads();
	    }
	     else if(common=='o')
	    {
	    	reado();
	    }
	     else if(common=='x')
	    {
	    	readx();
	    }
	    else
	    {
	    	printf("%c:%s\n", common,"don not know you command");
	    }

    }
	return 22;
}