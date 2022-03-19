/*
 * Environ.c
 *
 *  Created on: 2018年5月25日
 *      Author: uzi
 */
#include"Envrion.h"
extern char** environ;



int main()
{
	int i;
	for(i=0;environ[i]!=NULL;i++)
	{
		printf("%s\n",environ[i]);
	}
	
	printf("logname = %s \n",GetUserName());
	printf("pwd = %s \n",GetPwd());	
	return 0;
}
