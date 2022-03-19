#include"SessionManager.h"
#include <WtsApi32.h>
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>

std::vector<DWORD>  SessionManager::GetActiveSessionList()
{
	std::vector<DWORD> ret;
	PWTS_SESSION_INFO pSessionInfo = NULL;
	DWORD dwCount = 0;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);

	for (DWORD i = 0; i < dwCount; i++)
	{
		WTS_SESSION_INFO si = pSessionInfo[i];
		if (WTSActive == si.State)
		{
			ret.push_back(si.SessionId);
			break;
		}
	}
	WTSFreeMemory(pSessionInfo);
	return ret;
}
BOOL SessionManager::GetWTSSessions(vector<RDPLoginSessionInfo>& vec)
{
	WTS_SESSION_INFO *sessionInfo = NULL;
	DWORD sessionInfoCount;

	DWORD Reserved = 0;
	DWORD Version = 1;

	BOOL bRet = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, Reserved, Version, &sessionInfo, &sessionInfoCount);
	if (!bRet)
	{
		return bRet;
	}
	unsigned int userCount(0);

	for (unsigned int i = 0; i < sessionInfoCount; ++i)
	{
		if (sessionInfo[i].SessionId == 65536)  continue;
		RDPLoginSessionInfo info;

		info.sid = sessionInfo[i].SessionId;
		info.sName = sessionInfo[i].pWinStationName;
		info.state = sessionInfo[i].State;
		info.User = GetLoginUserInfo(sessionInfo[i].SessionId);
		userCount++;
		vec.push_back(info);
	}
	WTSFreeMemory(sessionInfo);
	return bRet;
}
BOOL SessionManager::WTSSend(DWORD session, TCHAR* content, TCHAR* title,DWORD& response)
{
	BOOL sendMsgBox = WTSSendMessage(
		WTS_CURRENT_SERVER_HANDLE,
		session,
		title, lstrlen(title) * sizeof(TCHAR),
		content, lstrlen(content) *sizeof(TCHAR),
		MB_YESNO,  //MB_OK
		10,
		&response,
		TRUE);

	//if (!sendMsgBox)
	//{
	//	printf("消息发送失败，错误代码：%d", GetLastError());
	//}
	//else
	//{
	//	switch (response)
	//	{
	//		/**
	//		case  IDABORT:
	//		case  IDCANCEL:
	//		case  IDCONTINUE:
	//		case  IDIGNORE:
	//		case  IDRETRY:
	//		case  IDTRYAGAIN:
	//		case  IDASYNC:
	//		**/
	//	case  IDYES:
	//		printf("用户收到消息，已点击 YES");
	//		break;
	//	case  IDNO:
	//		printf("用户收到消息，已点击 NO ");
	//		break;
	//	case  IDOK:
	//		printf("用户收到消息，已点击确认");
	//		break;
	//	case  IDTIMEOUT:
	//		printf("用户收到消息，但未点击确认，超时 10 秒已自动关闭");
	//		break;
	//	default:
	//		break;
	//	}
	//}
	return sendMsgBox;
}
BOOL SessionManager::WTSDisconnect(DWORD session)
{
	BOOL bRet = WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, session, FALSE);
	if (!bRet)
	{
		return bRet;
	}
}
BOOL SessionManager::WTSLogoff(DWORD session)
{
	BOOL bRet = WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, session, TRUE);
	if (!bRet)
	{
		return bRet;
	}
	bRet = WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, session, FALSE);
	return bRet;
}
HANDLE SessionManager::OpenServer(TCHAR* sName)
{
	return WTSOpenServer(sName);
}
void SessionManager::CloseServer(HANDLE hServer)
{
	WTSCloseServer(hServer);
}
stdstr SessionManager::GetInitProgram(TCHAR* sName, TCHAR* uName)
{
	TCHAR* data = NULL;
	DWORD len;
	WTSQueryUserConfig(sName, uName, WTSUserConfigInitialProgram, (TCHAR**)data, &len);
	stdstr str(data, len);
	WTSFreeMemory(data);
	return str;
}
BOOL SessionManager::SetInitProgram(TCHAR* sName, TCHAR* uName, TCHAR* initProgram)
{
	return WTSSetUserConfig(sName, uName, WTSUserConfigInitialProgram, initProgram, lstrlen(initProgram));
}
bool SessionManager::IsInRDP(DWORD session)
{
	WTS_INFO_CLASS WTSInfoClass;
	DWORD pCount = 0;
	TCHAR* ppBuffer;

	WTSInfoClass = WTSSessionInfo;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		session,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		WTSINFO* sInfo = (WTSINFO*)ppBuffer;

		if (lstrcmpi(sInfo->WinStationName, _T("Console")) != 0)
		{
			return true;
		}
	}
	return false;
}