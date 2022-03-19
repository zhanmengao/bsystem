#include<unistd.h>

int main(int argc,char* argv[])
{
	if(argc <3)
	{
		return -1;
	}
	link(argv[1],argv[2]);
	unlink(argv[1]);
	return 0;
}
