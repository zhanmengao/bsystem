#pragma once
#include<string>
using std::string;
static std::string SetTime(std::string Time)
{
	std::string year;
	char strRet[1024] = { 0 };
	if (Time != "*" && Time != "")
	{
		int i = 0, j = 0;
		int TimeSize = Time.length();
		while (i < TimeSize)
		{
			if (i == 4)
			{
				strRet[j + i] = '-';
				j++;
				strRet[j + i] = Time[i];
				i++;
				continue;
			}
			else if (i == 6)
			{
				strRet[j + i] = '-';
				j++;
				strRet[j + i] = Time[i];
				i++;
				continue;
			}
			strRet[j + i] = Time[i];
			i++;
		}
	}
	return year = strRet;
}
