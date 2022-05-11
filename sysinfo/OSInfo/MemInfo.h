//
// Created by uzi on 11/24/20.
//

#ifndef OSINFO_MEMINFO_H
#define OSINFO_MEMINFO_H
#include<string.h>
#include<stdio.h>
struct MemoryInfo
{
    char MemTotal[64];
};
void GetMemoryInfo(struct MemoryInfo* info)
{
    memset(info,0,sizeof(struct MemoryInfo));
    FILE* fp = fopen("/proc/meminfo","r");
    if(!fp)
    {
        perror("/proc/meminfo");
        return;
    }
    char szTest[1000] = {0};
    // read file line by line
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n

        char* str = strstr(szTest,"MemTotal");
        if(str != NULL)
        {
            if (strcmp(info->MemTotal, "") == 0)
            {
                char *str = strstr(szTest, ":");
                strcpy(info->MemTotal, str + 1);
            }
            continue;
        }
    }
    fclose(fp);
}
#endif //OSINFO_MEMINFO_H
