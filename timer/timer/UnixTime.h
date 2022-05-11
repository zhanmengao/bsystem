//
// Created by zhanmengao on 10/13/20.
//

#ifndef UNIXTIME_UNIXTIME_H
#define UNIXTIME_UNIXTIME_H
#include"../Time/Time.hpp"
#if __linux
#include <sys/time.h>
#include <unistd.h>

struct timeval GetTimeVal()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv;
}
long GetMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long millisecond = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return millisecond;
}

int SetTimer(struct timeval tv)
{
	struct itimerval newVal,oldVal;
	newVal.it_value = tv;
	newVal.it_interval.tv_sec=0;
	newVal.it_interval.tv_usec=0;
	setitimer(ITIMER_REAL,&newVal,&oldVal);
}
#endif
#endif //UNIXTIME_UNIXTIME_H
