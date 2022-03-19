 
/* ************************************************************************
*       Filename:  system_test.c
*    Description:
*        Version:  1.0
*        Created:  2015年04月12日 10时41分49秒
*       Revision:  none
*       Compiler:  gcc
*         Author:  YOUR NAME (),
*        Company:
* ************************************************************************/
 
 
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
int main( int argc, char *argv[] )
{
	pid_t	pid;
	int	ret	= 0;
	int	fd[2]	= { 0 };
 
/* 创建管道 */
	ret = pipe( fd );
	if ( ret == -1 )
	{
		perror( "pipe" );
		_exit( 1 );
	}
 
/* 创建子进程，目的  1exec 2复制管道文件描述符 */
	pid = vfork();
	if ( pid < 0 )
	{
		perror( "vfork" );
	}
	else if ( pid == 0 )
	{
		dup2( fd[1], 1 );                       /* 标准输出重定向到管道的写端 */
 
		char str[50] = "dmidecode -s system-serial-number";
		execlp( "/bin/sh", "sh", "-c", str, NULL );
	}
	else  
	{
		char result[100] = "";
		read( fd[0], result, sizeof(result) );  /* 从管道的读端读取数据 */
 
		char msg[100] = "";
		sprintf( msg, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",
			 result[7], result[8], result[10], result[11], result[13], result[14], result[16], result[17],
			 result[19], result[20], result[22], result[23], result[25],
			 result[26], result[28], result[29], result[31], result[32],
			 result[34], result[35], result[37], result[38], result[40],
			 result[41], result[43], result[44], result[46], result[47],
			 result[49], result[50], result[52], result[53] );
		printf( "---->%s\n", msg );
		printf( "---->%s\n", result );
	}
	return(0);
}
 
 
