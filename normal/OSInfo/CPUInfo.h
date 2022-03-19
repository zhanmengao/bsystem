//
// Created by uzi on 11/23/20.
//

#ifndef OSINFO_CPUINFO_H
#define OSINFO_CPUINFO_H
struct CPUInfo
{
    char name[128];
    char cpuMHZ[64];
    char cacheSize[64];
    char producer[64];
};
#ifdef __linux
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void GetCPUInfo(struct CPUInfo* info)
{
    memset(info,0,sizeof(struct CPUInfo));
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(NULL == fp)
        printf("failed to open cpuinfo\n");
    char szTest[1000] = {0};
    // read file line by line
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n

        char* str = strstr(szTest,"model name");
        if(str != NULL)
        {
            if( strcmp(info->name,"") == 0)
            {
                char* str = strstr(szTest,":");
                strcpy(info->name,str + 1);
            }
            continue;
        }

        str = strstr(szTest,"cpu MHz");
        if(str != NULL)
        {
            if( strcmp(info->cpuMHZ,"") == 0)
            {
                char* str = strstr(szTest,":");
                strcpy(info->cpuMHZ,str + 1);
            }
            continue;
        }

        str = strstr(szTest,"cache size");
        if(str != NULL)
        {
            if( strcmp(info->cacheSize,"") == 0)
            {
                char* str = strstr(szTest,":");
                strcpy(info->cacheSize,str + 1);
            }
            continue;
        }

        str = strstr(szTest,"vendor_id");
        if(str != NULL)
        {
            if( strcmp(info->producer,"") == 0)
            {
                char* str = strstr(szTest,":");
                strcpy(info->producer,str + 1);
            }
            continue;
        }

        str = strstr(szTest,"vendor_id");
    }
    fclose(fp);
}

#else
#endif
#endif //OSINFO_CPUINFO_H
