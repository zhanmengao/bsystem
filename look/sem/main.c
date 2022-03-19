#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include<string.h>
sem_t sem;
pthread_t thread;
void* change_resource(void *res)
{
    char *msg = (char*)res;

    while (1)
    {
        sem_wait(&sem);         //++
        strcat(msg, "a");
        printf("resource changed with value: %s\n", msg);
    }
}

int main()
{
    char resource[4096] = {0};
    int ret = sem_init(&sem, 0, 0);
    if (ret == -1)
    {
        printf("sem_init failed \n");
        return -1;
    }

    ret = pthread_create(&thread, NULL, change_resource, (void*)resource);
    if (ret != 0)
    {
        printf("pthread_create failed \n");
        return -1;
    }

    while (1)
    {
        sem_post(&sem);             //--
        sleep(1);
    }
}
