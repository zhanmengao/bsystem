#include "stdafx.h"
#include "SocketMsg.h"
#include <io.h>

#include "gclog.h"

#define ZPORT    23456

#ifdef _WIN64

#include <json_x64\json.h>

#ifdef _DEBUG
#pragma comment(lib, "./curl-7.59.0_x64/lib/libcurl-d_imp.lib")
#else
#pragma comment(lib, "./curl-7.59.0_x64/lib/libcurl_imp.lib")
#endif
#else
#include <json\json.h>
#pragma comment(lib,"libcurl.lib")
#ifdef _DEBUG
#pragma comment(lib,"jsoncpp.lib")
#else
#pragma comment(lib,"jsoncpp.lib")
#endif

#endif

#pragma comment(lib,"Iphlpapi.lib")

//UtilsTools utilsTools;
CallFunction callfunction;


/**
class TaskCallback
{
public:
	static void TaskCallback1(int result)
	{
		WriteLog("TaskCallback:: %d\n", result);
	}
};

// 不启用线程池 [ == ]

class Task
{
public:
**/
static int Task1(SOCKET sClient)
{

	char buff[2048 * 2] = { 0 };
	memset(buff, '\0', sizeof(buff) * sizeof(char));
	int nRecv = ::recv(sClient, buff, sizeof(buff) * sizeof(char), 0);


	int _code = 0;

	if (nRecv <= 0) return 0;

	Json::Reader reader;
	Json::Value  resp;

	int code;
	string msg;
	string data;

	try
	{
		if (!reader.parse(buff, resp, false))
		{
			//LOG(INFO) << "recv bad json format!";
			return FALSE;
		}
		code = atoi(resp["code"].asString().c_str());
		msg = resp["msg"].asString();
		data = resp["data"].asString();

		WriteLog("接收到数据 coed:%d msg:%s data:%s \r\n", code, msg.c_str(), data.c_str());

	}
	catch (...)
	{
		::send(sClient, "action:error", 16, 0);
		::closesocket(sClient);
		//LOG(INFO) << "Task1 Unknown exception!";
		return TRUE;
	}

	_code = code;


	int returnCode = 0;

	switch (code)
	{
	case 0:  //获取用户列表
		returnCode = Msg_GetUserList(sClient, code);
		break;

	case 1: //获取用户组列表
		returnCode = Msg_GetGroupList(sClient, code);
		break;

	case 2:
		break;

	case 103: //创建用户
		returnCode = Msg_AddSysUser(sClient, data, code);
		break;

	case 4: //删除用户
		returnCode = Msg_DelSysUser(sClient, data, code);
		break;

	case 5: //用户加入用户组
		returnCode = Msg_AddUserToGroup(sClient, data, code);
		break;

	case 6: //用户移出用户组
		returnCode = Msg_DelUserToGroup(sClient, data, code);
		break;

	case 7: //创建用户组
		returnCode = Msg_AddGroup(sClient, data, code);
		break;

	case 8: //删除用户组
		returnCode = Msg_DelGroup(sClient, data, code);
		break;

	case 10: //获取已安装软件列表
		returnCode = Msg_GetInstallAppList(sClient, code);
		break;

	case 11: //获取已发布的应用（注册表中已有的）
		returnCode = Msg_GetVEAppList(sClient, code);
		break;

	case 12: //添加（改写）虚拟应用（写注册表）
		returnCode = Msg_AppendVEApp(sClient, data, code);
		break;

	case 13: //删除已发布的应用（从注册表中删除）
		returnCode = Msg_DeleteVEApp(sClient, data, code);
		break;

	case 31: //探活
		returnCode = Msg_Keeplive(sClient, data, code);
		break;

	case 101: //禁用或启用用户
		returnCode = Msg_DisableSysUser(sClient, data, code);
		break;

	case 104: //批量导入用户
		returnCode = Msg_AddSysUsers2(sClient, data, code);
		break;

	case 106: //批量创建用户( 根据 前缀 + 编号 产生 )
		returnCode = Msg_AddSysUsers3(sClient, data, code);
		break;

	case 108: //批量移除用户( 根据 前缀 + 编号 产生 )
		returnCode = Msg_RemoveSysUsers3(sClient, data, code);
		break;

	default:
		break;
	}

	if (returnCode < 0)
	{

		Json::Value JsonRoot;
		JsonRoot["msg"] = "Error";
		JsonRoot["code"] = returnCode;
		JsonRoot["data"] = "NULL";


		Json::FastWriter writer;
		std::string josnstr = writer.write(JsonRoot);
		WriteLog("%s\n", josnstr.c_str());
		::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);
	}

	::closesocket(sClient);

	return TRUE;
}
/**
};

**/

int InitTeacherUser()
{

	callfunction.WebR_AddSysUser(L"Teacher0314", L"hcy9527@vesystem.com");
	callfunction.WebR_AddUserToGroup(L"Remote Desktop Users", L"Teacher0314");

	return 0;

}

int SocketInit()
{

	InitTeacherUser();

	BYTE minorVer = 2;
	BYTE majorVer = 2;

	/**
		int kernelNumber = utilsTools._GetNoOfProcessors();
		ThreadPool threadPool(kernelNumber,	kernelNumber*2);
	**/

	/**
	* 创建基本目录

		if (CreateShareFloder())
		{
			return FALSE;
		}
	*/
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(minorVer, majorVer);
	if (::WSAStartup(sockVersion, &wsaData) != 0)
	{
		return FALSE;
	}

	//snet_info netInfo = utilsTools.GetNetInfo();
	//string ip = netInfo.strIp;
	////LOG(INFO) << "本机ip" << ip.c_str();

	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		//LOG(INFO) << "Failed socket()";
		return FALSE;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(ZPORT);   //PORT: 23456
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		//LOG(INFO) << "Failed bind()" << WSAGetLastError();
		return FALSE;
	}

	//if (::listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	if (::listen(sListen, 2) == SOCKET_ERROR)
	{
		//LOG(INFO) << "Failed listen()";
		return FALSE;
	}

	while (StopAccept != 1)
	{

		SOCKET sClient = ::accept(sListen, NULL, NULL);

		if (sClient == INVALID_SOCKET)
		{
			//LOG(INFO) << "Failed accept()"<< WSAGetLastError();
			continue;
		}

		/**
		* 不启用线程池
		threadPool.QueueTaskItem(
			Task::Task1,
			sClient,
			TaskCallback::TaskCallback1,
			TRUE );
		**/

		Task1(sClient);

		if (StopAccept == 1)
		{
			break;
		}

		//Sleep( 50 );		
	}

	::closesocket(sListen);

	::WSACleanup();

	return 0;
}

/***
*
*======================================================================================================================**/
int Msg_GetUserList(SOCKET sClient, int msgcode)
{
	vector<CString> userlist;
	Json::Value userslist;

	callfunction.WebR_GetUserList(userlist);
	for (int i = 0; i < userlist.size(); i++)
	{
		Json::Value item;
		char *username = GStringClass::UnicodeToUtf8(userlist[i].GetBuffer());

		item["name"] = GStringClass::UnicodeToUtf8(GStringClass::MultiCharToWideChar(username));
		userslist.append(item);
	}
	//Json::FastWriter writer;
	//std::string josnstr = writer.write(root);
	//::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	Json::Value JsonRoot;
	JsonRoot["msg"] = "GetUserList";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = userslist;


	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	return 0;
}

int Msg_GetGroupList(SOCKET sClient, int msgcode)
{
	vector<CString> grouplist;
	Json::Value usergrouplist;

	callfunction.WebR_GetGroupList(grouplist);
	for (int i = 0; i < grouplist.size(); i++)
	{
		char *groupname = GStringClass::UnicodeToUtf8(grouplist[i].GetBuffer());
		//WriteLog("%s \r\n", groupname);

		Json::Value item;

		item["groupname"] = GStringClass::UnicodeToUtf8(GStringClass::MultiCharToWideChar(groupname));
		usergrouplist.append(item);

	}

	Json::Value JsonRoot;
	JsonRoot["msg"] = "GetGroupList";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = usergrouplist;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	return 0;
}

int Msg_AddGroup(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;
	//	try
	//	{
	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_AddGroup bad json format!";
		return -1;
	}

	//DWORD nRet = 0;
	string groupname = resp["groupname"].asString();
	nRet = callfunction.WebR_AddSysGroup(groupname.c_str());
	if (nRet != NERR_Success)
	{
		//LOG(INFO) << "Msg_AddGroup failed! " << " groupname:" << groupname.c_str() << "	status:" << nRet;
		return -2;
	}
	/**
		}

		catch (...)
		{
			//LOG(INFO) << "Msg_AddGroup Unknown exception!";
			return -3;
		}
	**/
	Json::Value JsonRoot;
	JsonRoot["msg"] = "Msg_AddGroup";
	JsonRoot["code"] = (long)nRet;
	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	return 0;
}

int Msg_DelGroup(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;


	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_DelGroup bad json format!";
		return -1;
	}

	string groupname = resp["groupname"].asString();
	nRet = callfunction.WebR_DelSysGroup(groupname.c_str());
	if (nRet != NERR_Success)
	{
		//LOG(INFO) << "Msg_DelGroup failed! " << " groupname:" << groupname.c_str() << "	status:" << nRet;
		return -2;
	}


	Json::Value JsonRoot;
	JsonRoot["msg"] = "DelGroup";
	JsonRoot["code"] = (long)nRet;
	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	return 0;
}

int Msg_DelUserToGroup(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;


	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_DelUserToGroup bad json format!";
		return -1;
	}


	string groupname = resp["groupname"].asString();
	string username = resp["username"].asString();

	nRet = callfunction.WebR_DelUserToGroup(groupname.c_str(), username.c_str());
	if (nRet != NERR_Success)
	{
		//LOG(INFO) << "Msg_DelUserToGroup failed! " << " groupname:" << groupname.c_str() << " username:" << username.c_str() << " status:" << nRet;
		return -2;
	}


	Json::Value JsonRoot;
	JsonRoot["msg"] = "DelUserToGroup";
	JsonRoot["code"] = (long)nRet;
	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	return 0;
}

int Msg_AddUserToGroup(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_AddUserToGroup bad json format!";
		return -1;
	}


	string groupname = resp["groupname"].asString();
	string username = resp["username"].asString();

	nRet = callfunction.WebR_AddUserToGroup(groupname.c_str(), username.c_str());
	if (nRet != NERR_Success)
	{
		//LOG(INFO) << "Msg_AddUserToGroup failed! " << " groupname:" << groupname.c_str() << " username:" << username.c_str() << " status:" << nRet;
		return -2;
	}


	Json::Value JsonRoot;
	JsonRoot["msg"] = "AddUserToGroup";
	JsonRoot["code"] = (long)nRet;
	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	return 0;
}

int Msg_DelSysUser(SOCKET sClient, string data, int msgcode)
{

	::CoInitialize(NULL);

	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;


	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_DelSysUser bad json format!";
		return -1;
	}

	string username = resp["username"].asString();

	nRet = callfunction.WebR_DelSysUser(username.c_str());
	if (nRet != NERR_Success)
	{
		//LOG(INFO) << "Msg_DelSysUser user failed! " << "  username:" << username.c_str() << "	status:" << nRet;
		return -2;
	}



	Json::Value JsonRoot;
	JsonRoot["msg"] = "DelSysUser";
	JsonRoot["code"] = (long)nRet;
	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	::CoUninitialize();

	return 0;
}

int Msg_AddSysUser(SOCKET sClient, string data, int msgcode)
{

	::CoInitialize(NULL);

	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	std::cout << "the json str:  " << data << std::endl;

	Json::Reader reader;
	Json::Value  resp;

	Json::Value JsonRoot;

	JsonRoot["msg"] = "CreateSysUser OK";
	JsonRoot["code"] = 1;


	if (!reader.parse(fjStr.c_str(), resp))
	{
		WriteLog("CreateSysUser bad json format!");
		return FALSE;
	}

	string UserName = resp["username"].asString();
	string KeyStr = resp["password"].asString();

	SetCfg* cfg = get_config();
	string panFu = cfg->disk;
	string HomeDirPath;
	CString cstr;
	string downloadPath;
	BOOL flag_1;
	string toExamine;
	BOOL flag_2;

	DWORD nStatus = 0;
	nStatus = callfunction.WebR_AddSysUser(UserName.c_str(), KeyStr.c_str());
	if (nStatus != NERR_Success)
	{
		WriteLog("create user failed!  username: %ls status: %d", UserName.c_str(), nStatus);

		if (nStatus == ERROR_ACCESS_DENIED)
		{

			JsonRoot["msg"] = "用户无权访问所请求的信息";

		}
		else if (nStatus == NERR_InvalidComputer)
		{

			JsonRoot["msg"] = "计算机名称无效";

		}
		else if (nStatus == NERR_NotPrimary)
		{

			JsonRoot["msg"] = "仅在域的主域控制器上允许该操作";
		}
		else if (nStatus == NERR_GroupExists)
		{


			JsonRoot["msg"] = "该组已经存在";

		}
		else if (nStatus == NERR_UserExists)
		{


			JsonRoot["msg"] = "用户帐户已存在";

		}
		else if (nStatus == NERR_PasswordTooShort)
		{

			JsonRoot["msg"] = "密码短于要求";
		}


		JsonRoot["code"] = -1;

		goto RETURN_MSG;
	}

	nStatus = callfunction.WebR_AddUserToGroup(L"Remote Desktop Users", UserName.c_str());
	if (nStatus != NERR_Success && nStatus != ERROR_MEMBER_IN_ALIAS)
	{
		WriteLog("Add to Remote Desktop Users fail! ");

		JsonRoot["msg"] = "Add to Remote Desktop Users fail";
		JsonRoot["code"] = -2;

		goto RETURN_MSG;
	}


RETURN_MSG:

	JsonRoot["data"] = "NULL";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	::CoUninitialize();

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////

int Msg_GetInstallAppList(SOCKET sClient, int msgcode)
{

	::CoInitialize(NULL);

	Json::Value tData;

	callfunction.WebR_GetInstallAppList(tData);


	Json::Value JsonRoot;
	JsonRoot["msg"] = "GetInstallAppList";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = tData;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	::CoUninitialize();

	return 0;
}

int Msg_GetVEAppList(SOCKET sClient, int msgcode)
{

	::CoInitialize(NULL);

	Json::Value tData;

	callfunction.WebR_GetVEAppList(tData);

	Json::Value JsonRoot;
	JsonRoot["msg"] = "GetVEAppList";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = tData;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	::CoUninitialize();

	return 0;
}

int Msg_AppendVEApp(SOCKET sClient, string data, int msgcode)
{

	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	/**
	{"Path":"D:/Program Files/Adobe/Adobe Flash CC 2015/Flash","RegName":"Flash","RequiredCommandLine":"","Name":"Adobe Flash Professional CC 2015"}

	**/


	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_AppendVEApp bad json format!";
		return -1;
	}

	::CoInitialize(NULL);

	int tCount = callfunction.WebR_AppendAppList(resp);

	Json::Value JsonRoot;
	JsonRoot["msg"] = "AppendVEApp";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = "";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	JsonRoot.clear();


	::CoUninitialize();


}

int Msg_DeleteVEApp(SOCKET sClient, string data, int msgcode)
{

	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		WriteLog("Msg_DeleteVEApp bad json format!");
		return -1;
	}

	::CoInitialize(NULL);

	int tCount = callfunction.WebR_DeleteVEAppList(resp);

	Json::Value JsonRoot;
	JsonRoot["msg"] = "DeleteVEApp";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = "";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	josnstr.clear();

	::CoUninitialize();

	return 0;

}

int Msg_Keeplive(SOCKET sClient, string data, int msgcode)
{
	Json::Value JsonRoot;
	JsonRoot["msg"] = "Keeplive";
	JsonRoot["code"] = 1;
	JsonRoot["data"] = "Keeplive";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	return TRUE;
}

int Msg_AddSysUsers2(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		WriteLog("Msg_AddSysUsers2 bad json format!");
		return -1;
	}

	::CoInitialize(NULL);

	Json::Value  JsonRoot;

	//{ "username":"batch1" }
	JsonRoot["code"] = 1;
	JsonRoot["msg"] = "AddSysUsers2";

	int usercount = resp.size();

	Json::Value items;
	for (int i = 0; i < usercount; i++)
	{

		Json::Value item;

		string username = resp[i]["username"].asString();
		string password = resp[i]["password"].asString();

		DWORD nStatus = 0;
		nStatus = callfunction.WebR_AddSysUser(username.c_str(), password.c_str());
		if (nStatus != NERR_Success)
		{
			char errorcode[10] = { 0 };
			itoa(nStatus, errorcode, 10);
			item["username"] = username;
			item["errorcode"] = errorcode;

			items.append(item);

			WriteLog("%ls", item.toStyledString().c_str());
			continue;
		}

		nStatus = callfunction.WebR_AddUserToGroup(L"Remote Desktop Users", username.c_str());
		if (nStatus != NERR_Success && nStatus != ERROR_MEMBER_IN_ALIAS)
		{

		}

	}

	::CoUninitialize();


	JsonRoot["data"] = items;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);


	::CoUninitialize();

	return 0;


}

int Msg_DisableSysUser(SOCKET sClient, string data, int msgcode)
{

	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_DisableSysUser bad json format!";
		return -1;
	}

	::CoInitialize(NULL);

	Json::Value item;

	string username = resp["username"].asString();
	int status = resp["status"].asInt();

	DWORD nStatus = 0;


	nStatus = callfunction.WebR_SetUserStatus(username.c_str(), status);

	Json::Value JsonRoot;

	if (status == 1)
	{
		JsonRoot["msg"] = "DisableSysUser";
	}
	else
	{
		JsonRoot["msg"] = "EnableSysUser";
	}

	JsonRoot["code"] = 1;
	JsonRoot["data"] = "";

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	josnstr.clear();

	::CoUninitialize();

	return 0;

}

int Msg_AddSysUsers3(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_AddSysUsers3 bad json format!";
		return -1;
	}

	::CoInitialize(NULL);

	Json::Value  JsonRoot;

	JsonRoot["code"] = 1;
	JsonRoot["msg"] = "AddSysUsers3";

	string UserNameX = resp["UserNameX"].asString();
	string SNumStr = resp["SNum"].asString();
	string ENumStr = resp["ENum"].asString();

	int SNum = atoi(SNumStr.c_str());
	int ENum = atoi(ENumStr.c_str());

	Json::Value items;

	for (int i = SNum; i <= ENum; i++)
	{

		Json::Value item;

		/**
		* 拼接用户名
		*/
		char tuserName[20] = { 0 };
		sprintf(tuserName, "%s%d", UserNameX.c_str(), i);

		/**
		* 用内置规则生成密码
		**/
		UCHAR preStr[16] = { 0 };
		char postStr[36] = { 0 };
		CalcMD5(tuserName, strlen(tuserName), preStr);
		Hex2Char(preStr, (PUCHAR)postStr, 8);
		char md5_8[10] = { 0 };
		strncpy(md5_8, postStr, 8);
		char md5_password[20] = { 0 };
		sprintf(md5_password, "Ve:%s", md5_8);


		DWORD nStatus = 0;
		nStatus = callfunction.WebR_AddSysUser(tuserName, md5_password);
		if (nStatus != NERR_Success)
		{
			char errorcode[10] = { 0 };
			itoa(nStatus, errorcode, 10);
			item["username"] = tuserName;
			item["errorcode"] = errorcode;

			items.append(item);

			WriteLog("%ls", item.toStyledString().c_str());

			continue;
		}

		nStatus = callfunction.WebR_AddUserToGroup(L"Remote Desktop Users", tuserName);
		if (nStatus != NERR_Success && nStatus != ERROR_MEMBER_IN_ALIAS)
		{
			continue;
		}

	}

	::CoUninitialize();

	JsonRoot["data"] = items;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);
	WriteLog("%s\n", josnstr.c_str());
	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	::CoUninitialize();

	return 0;


}

int Msg_RemoveSysUsers3(SOCKET sClient, string data, int msgcode)
{
	string fjStr = GStringClass::replace_all(data, "\\\\", "\\");
	fjStr = GStringClass::replace_all(data, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;

	DWORD nRet = 0;

	if (!reader.parse(fjStr.c_str(), resp))
	{
		//LOG(INFO) << "Msg_RemoveSysUsers3 bad json format!";
		return -1;
	}

	::CoInitialize(NULL);

	Json::Value  JsonRoot;

	JsonRoot["code"] = 1;
	JsonRoot["msg"] = "RemoveSysUsers3";

	string UserNameX = resp["UserNameX"].asString();
	string SNumStr = resp["SNum"].asString();
	string ENumStr = resp["ENum"].asString();

	int SNum = atoi(SNumStr.c_str());
	int ENum = atoi(ENumStr.c_str());

	Json::Value items;

	for (int i = SNum; i <= ENum; i++)
	{

		Json::Value item;

		/**
		* 拼接用户名
		*/
		char tuserName[20] = { 0 };
		sprintf(tuserName, "%s%d", UserNameX.c_str(), i);

		DWORD nStatus = 0;
		nStatus = callfunction.WebR_DelSysUser(tuserName);
		if (nStatus != NERR_Success)
		{
			char errorcode[10] = { 0 };
			itoa(nStatus, errorcode, 10);
			item["username"] = tuserName;
			item["errorcode"] = errorcode;

			items.append(item);

			continue;
		}

	}

	::CoUninitialize();

	JsonRoot["data"] = items;

	Json::FastWriter writer;
	std::string josnstr = writer.write(JsonRoot);

	WriteLog("%s\n", josnstr.c_str());

	::send(sClient, josnstr.c_str(), strlen(josnstr.c_str()), 0);

	::CoUninitialize();

	return 0;

}
