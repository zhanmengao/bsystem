#pragma once
#include<string>
#include<fstream>
#include<iostream>

#include<set>
#include<mutex>

#include"../../../System/Setting/Time/Time.hpp"

#ifdef _WIN32
#include"../../../System/File/FileManager/FileManager/FileManager.h"
#else
#include"../../../System/File/FileManager/LFileManager/FIleManager.h"

#endif
using namespace std;

class LogManager
{
public:
	LogManager()
	{
		//SetPath("");
	}
	LogManager(const string& logPath)
	{
		SetPath(logPath);
	}
	~LogManager()
	{
		Close();
	}
	void SetPath(const string& logFolder)
	{
		Close();
		mLogFolder = logFolder;
#ifdef _WIN32
		FileManager::CreatedMultipleDirectory(mLogFolder);
#elif __linux
		FileManager::CreateDir(mLogFolder.c_str());
#endif
		CheckLogPath();
	}
	void SetTag(const std::string& tag)
	{
		mTag = tag;
	}
	void SetWriteStdout(bool bSend)
	{
		bSend2STDOUT = bSend;
	}
	void SetDebug(bool bSend)
	{
		bSendDebug = bSend;
	}

	void Log(const string& log)
	{
		CheckLogPath();
		std::lock_guard<std::mutex> lock_a(mMonthLock);
		Log(log, mfs);
	}
	void Log(const string& log, fstream& fs)
	{
		string str = BuildLogHead();
		if (fs.is_open())
		{
			fs << str << log << endl;
		}
		if (bSend2STDOUT)
		{
			cout << str << log << endl;
		}
		if (bSendDebug)
		{
#ifdef _WIN32
			OutputDebugStringA(log.c_str());
#endif
		}
	}
	void Close()
	{
		if (mfs.is_open())
		{
			mfs.flush();
			mfs.close();
		}
		if (mfsDaily.is_open())
		{
			mfsDaily.flush();
			mfsDaily.close();
		}
	}
	//记录到当天的日志
	void LogDaily(const string& log)
	{
		CheckLogPath();
		//写
		std::lock_guard<std::mutex> lock_a(mDayLock);
		Log(log, mfsDaily);
	}
	//保留近n个月的日志,其他删除
	void DeleteLogMonth(int n)
	{
		std::lock(mMonthLock, mDayLock);             //原子操作，统统上锁
		//遍历添加n个月的目录文件夹
		set<std::string> saveFolder;
		time_t current = ::GetAnsiTime();
		for (int i = 0; i < n; i++)
		{
			//生成第n个月文件夹名称
			std::string str = GetMonthFolder(current - 60 * 60 * 24 * 30 * i);
			saveFolder.insert(str);
		}
		//遍历目录
		std::vector<FileInfo> outFileList;
		FileManager::GetAllFileInfo(mLogFolder.c_str(), outFileList);
		for (auto it = outFileList.begin(); it != outFileList.end(); it++)
		{
#ifdef _WIN32
			string folderName = mLogFolder + it->name + "\\";
#elif __linux
			string folderName = mLogFolder + it->fileName + "\\";
#endif
			//若没有，直接删
			if (saveFolder.find(folderName) == saveFolder.end())
			{
#ifdef __linux
				FileManager::Remove(folderName.c_str());
#elif _WIN32
				if (FileAttrHelper::IsFolder(it->attrib))
				{
					FileManager::RemoveFloder(folderName);

				}
				//文件，直接删
				else
				{
					FileManager::RemoveFile(mLogFolder + it->name);
				}
#endif
			}

		}
		mMonthLock.unlock();
		mDayLock.unlock();
	}
	string GetMonthFolder()
	{
		return GetMonthFolder(::GetAnsiTime());
	}
private:
	string BuildLogHead()
	{
		string str = TimeUtils::GetStandardTime(::GetAnsiTime());
		char buf[1024] = { 0 };
#ifdef _WIN32
		sprintf_s(buf, sizeof(buf), "%s %s:    ",BuildTag().c_str(), str.c_str());
#elif __linux
		sprintf(buf, "[%s]:    ", str.c_str());
#endif
		return buf;
	}
	string BuildTag()
	{
		return "[" + mTag + "]";
	}
	string GetMonthFolder(time_t t)
	{
		return mLogFolder + TimeUtils::GetTimeNumYm(t) + "\\";
	}
	string GetDailyPath()
	{
		return GetMonthFolder() + TimeUtils::GetTimeNumYmD(::GetAnsiTime()) + ".log";
	}
	string GetMonthLogPath()
	{
		return GetMonthFolder() + "log.txt";
	}
	void CheckLogPath()
	{
		std::lock(mMonthLock, mDayLock);             //原子操作，统统上锁
		tm current = ::GetTime(GetAnsiTime());

		//天数不对，重新打开文件
		if (!::CompareDay(mCurrentTime, current))
		{
			//月份不对，重新打开文件（天数不对可能导致月份不对）
			if (!::CompareDay(mCurrentTime, current))
			{
#ifdef _WIN32
				FileManager::CreateFloder(GetMonthFolder());
#elif __linux
				FileManager::CreateDir(GetMonthFolder().c_str());
#endif
				if (mfs.is_open())
				{
					mfs.close();
				}
				mfs.open(GetMonthLogPath(), ios::app);
			}
			if (mfsDaily.is_open())
			{
				mfsDaily.close();
			}
			string daily = GetDailyPath();
			mfsDaily.open(daily, ios::app);
		}
		mCurrentTime = current;
		mMonthLock.unlock();
		mDayLock.unlock();
	}
private:
	std::string mLogFolder;
	std::string mTag = "Default";
	fstream mfs;
	fstream mfsDaily;
	tm mCurrentTime = { 0 };
	mutex mMonthLock;
	mutex mDayLock;
	bool bSend2STDOUT = false;
	bool bSendDebug = true;
};
