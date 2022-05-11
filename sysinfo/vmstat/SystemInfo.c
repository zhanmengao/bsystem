
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include"Hardware.h"
    // read the linux config files to get system info  
      
    void getOsInfo()  
    {  
        FILE *fp = fopen("/proc/version", "r");    
        if(NULL == fp)     
            printf("failed to open version\n");     
        char szTest[1000] = {0};    
        while(!feof(fp))    
        {    
            memset(szTest, 0, sizeof(szTest));    
            fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n  
            printf("%s", szTest);    
        }    
        fclose(fp);   
    }  
      
    void getCpuInfo()  
    {  
        FILE *fp = fopen("/proc/cpuinfo", "r");    
        if(NULL == fp)     
            printf("failed to open cpuinfo\n");     
        char szTest[1000] = {0};     
        // read file line by line   
        while(!feof(fp))    
        {    
            memset(szTest, 0, sizeof(szTest));    
            fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n  
            printf("%s", szTest);    
        }    
        fclose(fp);   
    }  
      
    void getMemoryInfo()  
    {  
        FILE *fp = fopen("/proc/meminfo", "r");    
        if(NULL == fp)     
            printf("failed to open meminfo\n");     
        char szTest[1000] = {0};    
        while(!feof(fp))    
        {    
            memset(szTest, 0, sizeof(szTest));    
            fgets(szTest, sizeof(szTest) - 1, fp);   
            printf("%s", szTest);    
        }    
        fclose(fp);   
    }  
      
    int main(int argc, char **argv)  
    {  
        printf("===os information===\n");  
        getOsInfo();  
      
        printf("===cpu infomation===\n");  
        getCpuInfo();  
      
        printf("===memory information===\n");  
        getMemoryInfo();  
      
		struct BIOSInfo biosInfo = GetBIOSInfo();
		printf("bios ser: %s \n",biosInfo.serialnumber.c_str());
        return 0;  
    }  
