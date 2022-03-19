#include"SysDefine.h"
#include"../mstdio.h"
void minpscanf(const char* fmt,...)//fmt:%d %d %s, ...:some adress
{
	va_list ap;
	char *p;

	char *strp;
	char buffer[128];
	int* ip;
	double* dp;

	va_start(ap,fmt);// ap point head

	memset(buffer,0,128);
	for(p=fmt;(*p);p++)
	{
		if(*p!='%')
		{
			continue;
		}
		switch(*++p)
		{
		case 'd':
			ip=va_arg(ap,int*);
			gets(&buffer);
			printf("p=d,input=%s\n",buffer);
			*ip=atoi(buffer);
			break;
		case 'u':
			ip=va_arg(ap,int*);
			gets(&buffer);
			printf("p=y,input=%s\n",buffer);
			*ip=atoi(buffer);
			break;
		case 's':
		    strp=va_arg(ap,char*);
		    gets(strp);
		    printf("p=s,input=%s\n",strp);
			break;
		case 'f':
		    dp=va_arg(ap,double*);
			gets(&buffer);
			*dp=atof(buffer);
			printf("p=f,input=%s\n",buffer);
			break;
		case 'c':
		    strp=va_arg(ap,char*);
		    *strp=getc(STDIN_FILENO);
		    printf("p=c,input=%s\n",strp);
			break;
		default:
			break;
		}
	}
	va_end(ap);
}
int mainMyScanf()
{
	int i=0;
	char str[12];
	char c;
	double d=0;
	minpscanf("%d %s %f",&i,str,&d);
	printf("i=%d,str=%s,d=%f\n",i,str,d);
}