#include<stdio.h>
#include<dirent.h>
#include<string.h>

int main(int argc,char *argv[])
{
	char path[260] = {0};
	if(argc <=1)
	{
		getcwd(path,sizeof(path));
	}
	else
	{
		strcpy(path,argv[1]);
	}
	DIR* dir = opendir(path);
	if(dir == NULL)
	{
		perror(path);
		return -1;
	}
	struct dirent* dentry = NULL;
	while((dentry = readdir(dir)) != NULL)
	{
		printf("%s	",dentry->d_name);
	}
	printf("\n");
	closedir(dir);
	return 0;
}
