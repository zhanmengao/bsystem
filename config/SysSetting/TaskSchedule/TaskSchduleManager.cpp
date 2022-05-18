#include"TaskSchduleManager.h"
#include<iostream>
#define CHECK(hr) 	if (FAILED(hr))\
{\
return FALSE;\
}
TaskSchduleManager::TaskSchduleManager()
{
	m_lpITS = NULL;
	m_lpRootFolder = NULL;
	//��ʼ��COM
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return;
	}
	//�����������ʵ��
	hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (LPVOID*)(&m_lpITS));
	if (FAILED(hr))
	{
		return;
	}
	// ���ӵ��������Task Service��
	hr = m_lpITS->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr))
	{
		return;
	}
	//��ȡRoot Task Folder��ָ�룬ָ��ָ����ע�������
	hr = m_lpITS->GetFolder(_bstr_t("\\"), &m_lpRootFolder);
	if (FAILED(hr))
	{
		return;
	}
}

TaskSchduleManager::~TaskSchduleManager()
{
	if (m_lpITS)
	{
		m_lpITS->Release();
	}
	if (m_lpRootFolder)
	{
		m_lpRootFolder->Release();
	}
	// ж��COM
	::CoUninitialize();
}

BOOL TaskSchduleManager::NewTask(const TCHAR *lpszTaskName, const TCHAR *lpszProgramPath,
	const TCHAR *lpszParameters, const TCHAR *lpszAuthor)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	if (NULL == m_lpITS)
	{
		return FALSE;
	}
	// ���������ͬ�ļƻ�������ɾ��
	Delete(lpszTaskName);
	//�������������
	ITaskDefinition *pTaskDefinition = NULL;
	HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
	CHECK(hr);
	//����ע����Ϣ
	IRegistrationInfo *pRegInfo = NULL;
	CComVariant variantAuthor(NULL);
	variantAuthor = lpszAuthor;
	hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
	CHECK(hr);
	hr = pRegInfo->put_Author(variantAuthor.bstrVal);																//������Ϣ
	CHECK(hr);

	//���õ�¼���ͺ�����Ȩ��
	IPrincipal *pPrincipal = NULL;
	hr = pTaskDefinition->get_Principal(&pPrincipal);
	CHECK(hr);
	hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);									//��¼����
	CHECK(hr);
	hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);														//����Ȩ�ޣ����Ȩ��

	//����������Ϣ
	ITaskSettings *pSettting = NULL;
	hr = pTaskDefinition->get_Settings(&pSettting);
	CHECK(hr);
	hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
	hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
	hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);

	//����ִ�ж���
	IActionCollection *pActionCollect = NULL;
	hr = pTaskDefinition->get_Actions(&pActionCollect);
	CHECK(hr);
	IAction *pAction = NULL;
	hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollect->Release();

	//����ִ�г���·���Ͳ���
	CComVariant variantProgramPath(NULL);
	CComVariant variantParameters(NULL);
	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
	pAction->Release();
	variantProgramPath = lpszProgramPath;
	variantParameters = lpszParameters;
	pExecAction->put_Path(variantProgramPath.bstrVal);
	pExecAction->put_Arguments(variantParameters.bstrVal);
	pExecAction->Release();

	//������������ʵ���û���½������ */
	ITriggerCollection *pTriggers = NULL;
	hr = pTaskDefinition->get_Triggers(&pTriggers);
	CHECK(hr);
	ITrigger *pTrigger = NULL;
	hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
	if (SUCCEEDED(hr))
	{
		pTrigger->Release();
	}
	//ע������ƻ� 
	IRegisteredTask *pRegisteredTask = NULL;
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	hr = m_lpRootFolder->RegisterTaskDefinition(variantTaskName.bstrVal,
		pTaskDefinition,
		TASK_CREATE_OR_UPDATE,
		_variant_t(),
		_variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN,
		_variant_t(""),
		&pRegisteredTask);
	if (FAILED(hr))
	{
		pTaskDefinition->Release();
		return FALSE;
	}
	pTaskDefinition->Release();
	pRegisteredTask->Release();
	return TRUE;
}
BOOL TaskSchduleManager::GetTaskList(std::vector<_TaskSchduleInfo>& vec)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	IRegisteredTaskCollection *coll = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTasks(TASK_ENUM_HIDDEN, &coll);
	CHECK(hr);
	LONG nCount = 0;
	hr = coll->get_Count(&nCount);
	CHECK(hr);
	IRegisteredTask *pTask = NULL;
	VARIANT vtProp = { 0 };
	vtProp.vt = VT_I2;
	CComVariant variantEnable(NULL);
	for (int i = 1; i <= nCount; i++)
	{
		//��ȡ��Ϣ
		vtProp.iVal = i;
		hr = coll->get_Item(vtProp, &pTask);
		if (FAILED(hr))
		{
			continue;
		}
		_TaskSchduleInfo info;
		hr = pTask->get_Name(&variantEnable.bstrVal);
		if (SUCCEEDED(hr))
		{
			info.taskName = _com_util::ConvertBSTRToString(variantEnable.bstrVal);
		}
		hr = pTask->get_Path(&variantEnable.bstrVal);
		if (SUCCEEDED(hr))
		{
			info.path = _com_util::ConvertBSTRToString(variantEnable.bstrVal);
		}
		ITaskDefinition *taskDefinition = NULL;
		hr = pTask->get_Definition(&taskDefinition);
		if (SUCCEEDED(hr))
		{
			ITriggerCollection *pTriggers = NULL;
			hr = taskDefinition->get_Triggers(&pTriggers);
			if (SUCCEEDED(hr))
			{
				LONG triggersCount = 0;
				pTriggers->get_Count(&triggersCount);
				for (int j = 1; j <= triggersCount; j++)
				{
					ITrigger *trigger;
					hr = pTriggers->get_Item(j, &trigger);
					if (SUCCEEDED(hr))
					{
						TASK_TRIGGER_TYPE2 type;
						hr = trigger->get_Type(&type);
						if (SUCCEEDED(hr))
						{
							info.triggerVec.push_back(type);
						}
						trigger->Release();
					}
				}
				pTriggers->Release();
			}
			taskDefinition->Release();
		}
		pTask->Release();
		vec.push_back(info);
	}
	coll->Release();

	return TRUE;
}
// ����ָ������ƻ�
BOOL TaskSchduleManager::Run(const TCHAR *lpszTaskName, const TCHAR *lpszParam)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantParameters(NULL);
	variantTaskName = lpszTaskName;
	variantParameters = lpszParam;

	// ��ȡ����ƻ�
	IRegisteredTask *pRegisteredTask = NULL;
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ����
	hr = pRegisteredTask->Run(variantParameters, NULL);
	if (FAILED(hr))
	{
		pRegisteredTask->Release();
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}
BOOL TaskSchduleManager::Delete(const TCHAR *lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	HRESULT hr = m_lpRootFolder->DeleteTask(variantTaskName.bstrVal, 0);
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL TaskSchduleManager::DeleteFolder(const TCHAR *lpszFolderName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	CComVariant variantFolderName(NULL);
	variantFolderName = lpszFolderName;
	HRESULT hr = m_lpRootFolder->DeleteFolder(variantFolderName.bstrVal, 0);
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}
BOOL TaskSchduleManager::SetEnable(const TCHAR *lpszTaskName, BOOL bEnable)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	variantEnable = bEnable;                            // �Ƿ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// �����Ƿ�����
	pRegisteredTask->put_Enabled(variantEnable.boolVal);
	pRegisteredTask->Release();
	return TRUE;
}

BOOL TaskSchduleManager::IsEnable(const TCHAR *lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ��ȡ�Ƿ��Ѿ�����
	pRegisteredTask->get_Enabled(&variantEnable.boolVal);
	pRegisteredTask->Release();
	if (ATL_VARIANT_FALSE == variantEnable.boolVal)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL TaskSchduleManager::IsExist(const TCHAR *lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}

BOOL TaskSchduleManager::IsTaskValid(const TCHAR *lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ��ȡ����״̬
	TASK_STATE taskState;
	hr = pRegisteredTask->get_State(&taskState);
	if (FAILED(hr))
	{
		pRegisteredTask->Release();
		return FALSE;
	}
	pRegisteredTask->Release();
	// ��Ч
	if (TASK_STATE_DISABLED == taskState)
	{
		return FALSE;
	}

	return TRUE;
}
