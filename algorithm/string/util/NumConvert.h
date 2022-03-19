#ifndef NUMCONVERT_H
#define NUMCONVERT_H
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<vector>
#include<string.h>
#include<iostream>
class NumConvert
{
public:
    static std::string to16(int num)
    {
        std::vector<unsigned> rlist;
        unsigned quo =  num /16;
        unsigned rem = num%16;
        while(rem!=0)
        {
            rlist.push_back(rem);
            num = quo;
            quo = num /16;
            rem = num%16;
        }
        std::string ret("0x");
        char buffer[16] ={0};
        for(int i = rlist.size()-1;i>=0;i--)
        {
            std::cout<<rlist[i]<<std::endl;
            memset(buffer,0,sizeof(buffer));
            sprintf(buffer,"%02d",rlist[i]);
            ret+=buffer;
        }
        return ret;
    }
};

#endif
