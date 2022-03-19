#include <iostream>
#include <stdarg.h>
using namespace std;
void simple_va_fun(int i, ...);

int mainArgv(int argc, char *argv[])
{
	simple_va_fun(100);						//100 ? ?
	simple_va_fun(100, 200);				//100 200 ?
	simple_va_fun(100, 200, 'a');		//100 200 a
	return 0;
}

void simple_va_fun(int i, ...)
{
	va_list   arg_ptr;   //定义可变参数指针 
	va_start(arg_ptr, i);				// i为最后一个固定参数
	int j = va_arg(arg_ptr, int);				//返回第一个可变参数，类型为int
	char c = va_arg(arg_ptr, char);				 //返回第二个可变参数，类型为char
	va_end(arg_ptr);					 //  清空参数指针
	printf("%d %d %c\n", i, j, c);
	return;
}