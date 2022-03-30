#include <stdio.h>
#include"UnixTime.h"
int main()
{
    struct timeval t = GetTimeVal();
    printf("%ld %ld\n",t.tv_sec,t.tv_usec);
    printf("%s\n",GetCTime(GetAnsiTime()));
    char buf[100];
    GetStandardTime(GetTime(GetAnsiTime()),buf,sizeof(buf),"%Y-%m-%d %H:%M:%S");
    printf("%s\n",buf);
    return 0;
}
