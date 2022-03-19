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
	// ��������ƻ�
	BOOL NewTask(const TCHAR *lpszTaskName, const TCHAR *lpszProgramPath, const TCHAR *lpszParameters=_T(""),
		const TCHAR *lpszAuthor = _T(""));
	BOOL Run(const TCHAR *lpszTaskName, const TCHAR *lpszParam);															// ����ָ������ƻ�
	BOOL IsEnable(const TCHAR *lpszTaskName);																								// �ж�ָ������ƻ��Ƿ�����
	BOOL IsExist(const TCHAR *lpszTaskName);																									// �ж�ָ������ƻ��Ƿ����
	BOOL IsTaskValid(const TCHAR *lpszTaskName);																							// �ж�ָ������ƻ�״̬�Ƿ���Ч
	BOOL SetEnable(const TCHAR *lpszTaskName, BOOL bEnable);																	// ����ָ������ƻ��Ƿ��������ǽ���
	// ɾ��ָ������ƻ�
	BOOL Delete(const TCHAR *lpszTaskName);
	BOOL DeleteFolder(const TCHAR *lpszFolderName);
	//��ȡȫ��
	BOOL GetTaskList(std::vector<_TaskSchduleInfo>& vec);
private:
	ITaskService *m_lpITS;
	ITaskFolder *m_lpRootFolder;

};