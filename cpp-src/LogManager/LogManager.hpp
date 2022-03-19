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
	//��¼���������־
	void LogDaily(const string& log)
	{
		CheckLogPath();
		//д
		std::lock_guard<std::mutex> lock_a(mDayLock);
		Log(log, mfsDaily);
	}
	//������n���µ���־,����ɾ��
	void DeleteLogMonth(int n)
	{
		std::lock(mMonthLock, mDayLock);             //ԭ�Ӳ�����ͳͳ����
		//�������n���µ�Ŀ¼�ļ���
		set<std::string> saveFolder;
		time_t current = ::GetAnsiTime();
		for (int i = 0; i < n; i++)
		{
			//���ɵ�n�����ļ�������
			std::string str = GetMonthFolder(current - 60 * 60 * 24 * 30 * i);
			saveFolder.insert(str);
		}
		//����Ŀ¼
		std::vector<FileInfo> outFileList;
		FileManager::GetAllFileInfo(mLogFolder.c_str(), outFileList);
		for (auto it = outFileList.begin(); it != outFileList.end(); it++)
		{
#ifdef _WIN32
			string folderName = mLogFolder + it->name + "\\";
#elif __linux
			string folderName = mLogFolder + it->fileName + "\\";
#endif
			//��û�У�ֱ��ɾ
			if (saveFolder.find(folderName) == saveFolder.end())
			{
#ifdef __linux
				FileManager::Remove(folderName.c_str());
#elif _WIN32
				if (FileAttrHelper::IsFolder(it->attrib))
				{
					FileManager::RemoveFloder(folderName);

				}
				//�ļ���ֱ��ɾ
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
		std::lock(mMonthLock, mDayLock);             //ԭ�Ӳ�����ͳͳ����
		tm current = ::GetTime(GetAnsiTime());

		//�������ԣ����´��ļ�
		if (!::CompareDay(mCurrentTime, current))
		{
			//�·ݲ��ԣ����´��ļ����������Կ��ܵ����·ݲ��ԣ�
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
