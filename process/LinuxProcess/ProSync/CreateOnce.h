#ifndef _CREATEONCE_H
#define _CREATEONCE_H

#include<stdlib.h>
#include<stdio.h>
#include<sys/type.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<signalh>

int check_pid(char* pid_file)
{
	struct stat stb;
	FILE* pidfile;
	if(stat(pid_file,&stb) == 0)
	{
		pidfile = fopen(pid_file,"r")
			if(pidfile)
			{
				char buf[64];
				pid_t pid = 0;
				memset(buf,0,sizeof(buf));
				if(fread(buf,1,sizeof(buf),pidfile)
				{
						buf[sizeof(buf)-1] = '\0';
						pid = atoi(buf);
				}
				fclose(pidfile);
				//检查进程
				if(pid && kill(pid,0) == 0)
				{
					return 1;
				}
				printf("removeing pidfile %s ,process not running",pid_file);
				unlink(pid_file);			
			}
	return 0;
}

#endif
