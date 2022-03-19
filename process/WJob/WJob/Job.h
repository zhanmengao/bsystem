#pragma once
#include"JobUtils.h"


class Job
{
public:
	BOOL CreateJob(const TCHAR* jobName)
	{
		hJob = CreateJobObject(NULL, jobName);
		return hJob != NULL;
	}
	void SetUserTime()
	{

	}
private:
	HANDLE hJob;
};