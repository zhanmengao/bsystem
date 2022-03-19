#include"ThreadUtils.h"

#include<stdio.h>
#include<stdlib.h>



#ifndef __linux
void ThreadMain(void* argv)
{
	printf("I am thread %ld \n", pthread_self());
	//ThreadDetach(GetThreadID());
}
int main()
{
	//HANDLE tid =thread_create(ThreadMain, (void*)999);
	//ThreadWait(tid,1);
	return 0;
}
#else
void* ThreadMain(void* argv)
{
    printf("I am thread %ld %d\n", GetThreadID(),argv);
    //ThreadDetach(GetThreadID());
    return 0;
}
int main()
{
    pthread_t tid = NewThread(ThreadMain,(void*)999);
    printf("thread ret = %d \n",ThreadWait(tid));
    return 0;
}
#endif
