#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("STDIN_FILENO = %s\n",ttyname(0));
	printf("STDOut_FILENO = %s\n",ttyname(1));
	printf("fd2 = %s\n",ttyname(2));
	return 0;
}