#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
int Sprite(char* buffer,int length,char** res)
{
	int i=0,preIndex=0;
	int j=0;
	for(i=0;i<length;i++)
	{
		if(buffer[i]==' ' || buffer[i]=='\0')
		{
			int len=i-preIndex;
			memcpy(res[j],buffer+preIndex,len);
			preIndex=i+1;
			j++;
		}
	}
	memcpy(res[j],buffer+preIndex,length-preIndex);
	j++;
	return j;
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	char buffer[128];
	int fd;
	while(1)
	{
		if(gets(buffer))
		{
			printf("receiver command:   %s\n",buffer);
			pid=fork();
			if(pid==0)
			{
		        char *agrs[20];
				int begin=0;//buffer pre Index
				int j=0;// args now
				int i=0;
				int isCatch=0;
				char *p;
				char* filePath;
				for(p=buffer;(*p);p++,i++)
				{
					switch(*p)
					{
					//sprite	
					case ' ':
					    *p='\0';
					    agrs[j]=buffer+begin;
						j++;
						begin=i+1;
						isCatch=1;
						break;
					case '>':
					//output
					    p+=2;
					    filePath=p;
					    if(*p!=' ')
					    {
					    	p++;
					    	i++;
					    }
					    else
					    {
					    	*p='\0';
					    }
					    fd=open(filePath,O_WRONLY|O_CREAT,0644);
					    if(fd)
					    {
	                         dup2(fd,STDOUT_FILENO);//输出重定向
					    }
					    p++;
					    i++;
		        	    begin=i;
					    break;
					 case '<':
					//input
					    p+=2;
					    filePath=p;
					    if(*p!=' ')
					    {
					    	p++;
					    	i++;
					    }
					    else
					    {
					    	*p='\0';
					    }
					    fd=open(filePath,O_RDONLY|O_CREAT,0644);
					    if(fd)
					    {
	                        dup2(fd,STDIN_FILENO);//输出重定向
					    }
					    p++;
					    i++;
		        	    begin=i;
					    break;
					}
				}
				if(!isCatch)
				{
				    agrs[j++]=buffer+begin;
				}
				agrs[j]=NULL;
				execvp(agrs[0],agrs);
			}
		}
		int state;
		waitpid(-1,&state,0);
		if(WIFEXITED(state))
		{
			printf("exited: %d\n",WEXITSTATUS(state));
		}
		else if(WIFSIGNALED(state))
		{
			printf("signaled: %d\n",WTERMSIG(state));
		}
	}
	return 0;
}