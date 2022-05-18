#pragma once
#include <Atlbase.h>
#include <comdef.h>
#include <taskschd.h>
#include<vector>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#pragma comment(lib, "taskschd.lib")

struct _TaskSchduleInfo
{
	std::string taskName;
	std::string path;
	std::vector<_TASK_TRIGGER_TYPE2> triggerVec;
};
class TaskSchduleManager
{
public:
	TaskSchduleManager();
	~TaskSchduleManager();
public:
	// 创建任务计划
	BOOL NewTask(const TCHAR *lpszTaskName, const TCHAR *lpszProgramPath, const TCHAR *lpszParameters=_T(""),
		const TCHAR *lpszAuthor = _T(""));
	BOOL Run(const TCHAR *lpszTaskName, const TCHAR *lpszParam);															// 运行指定任务计划
	BOOL IsEnable(const TCHAR *lpszTaskName);																								// 判断指定任务计划是否启动
	BOOL IsExist(const TCHAR *lpszTaskName);																									// 判断指定任务计划是否存在
	BOOL IsTaskValid(const TCHAR *lpszTaskName);																							// 判断指定任务计划状态是否有效
	BOOL SetEnable(const TCHAR *lpszTaskName, BOOL bEnable);																	// 设置指定任务计划是否启动还是禁用
	// 删除指定任务计划
	BOOL Delete(const TCHAR *lpszTaskName);
	BOOL DeleteFolder(const TCHAR *lpszFolderName);
	//获取全部
	BOOL GetTaskList(std::vector<_TaskSchduleInfo>& vec);
private:
	ITaskService *m_lpITS;
	ITaskFolder *m_lpRootFolder;

};