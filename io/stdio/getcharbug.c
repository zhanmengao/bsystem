#include <stdio.h>
#define MAXLINE 1024

int
main(void)
{
	char	c;

	while ((c = getchar()) != EOF)
		putchar(c);
}
