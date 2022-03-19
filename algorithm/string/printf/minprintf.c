#include <stdio.h>
#include <stdarg.h>
#include"../mstdio.h"

void minprintf(const char* fmt,...)
{
	va_list ap;
	char *p,*sval;
	int ival;
	double dval;

	va_start(ap,fmt);// ap point head

	for(p=fmt;(*p);p++)
	{
		if(*p!='%')
		{
			putchar(*p);
			continue;
		}
		switch(*++p)
		{
			case 'd':
			{
				ival = va_arg(ap, int);
				printf("%d", ival);
				break;
			}

			case 's':
			{
				for (sval = va_arg(ap, char*); *sval; sval++)
				{
					putchar(*sval);
				}
				break;
			}
			case 'f':
			{
				dval = va_arg(ap, double);
				printf("%f", dval);
				break;
			}
			default:
				break;
		}
	}
	va_end(ap);
}

int mainMyprintf()
{
	minprintf("%s","i am marin");
	return 0;
}