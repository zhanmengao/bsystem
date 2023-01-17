//
// Created by wby on 23-1-16.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "pidstat.h"

int main(int argc,char *argv[]){
    int pid = atoi(argv[1]);
    printf("begin watch %d cpu \n",pid);
    while (1){
        printf("%f\n",get_proc_cpu(pid));
        usleep(100000);
    }
    return 0;
}
