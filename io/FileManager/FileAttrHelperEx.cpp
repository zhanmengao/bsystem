#include <stdio.h>
#include <bitset>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <iostream>
#include"FileAttrHelperEx.h"
#include <atlstr.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <atlsecurity.h>
#include<io.h>
#include<set>
using std::bitset;
using std::string;
using std::cout;
using std::endl;

//ʹ��Windows��HeapAlloc�������ж�̬�ڴ����
#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

typedef BOOL(WINAPI *SetSecurityDescriptorControlFnPtr)(
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);
typedef BOOL(WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl, DWORD dwAceRevision,
	DWORD AceFlags, DWORD AccessMask, PSID pSid);

//     Ϊ�ļ�(Ŀ¼)���һ���ʻ����飩��Ȩ��
//     TCHAR *lpszFileName     �ļ���Ŀ¼��
//     TCHAR *lpszAccountName  �ʻ����飩
//       DWORD dwAccessMask       Ȩ������(��GENERIC_ALL��GENERIC_READ��)
BOOL FileAttrHelperEx::EnableFileAccountPrivilege(TCHAR *lpszFileName, TCHAR *lpszAccountName, DWORD dwAccessMask)
{
	// ����SID����
	SID_NAME_USE   snuType;
	// ������LookupAccountName��صı�����ע�⣬ȫΪ0��Ҫ�ڳ����ж�̬���䣩
	TCHAR *        szDomain = NULL;
	DWORD          cbDomain = 0;
	LPVOID         pUserSID = NULL;
	DWORD          cbUserSID = 0;
	// ���ļ���صİ�ȫ������ SD �ı���
	PSECURITY_DESCRIPTOR pFileSD = NULL;     // �ṹ����
	DWORD          cbFileSD = 0;

	// һ���µ�SD�ı��������ڹ����µ�ACL�������е�ACL����Ҫ�¼ӵ�ACL����������
	SECURITY_DESCRIPTOR  newSD;
	// ��ACL ��صı���
	PACL           pACL = NULL;
	BOOL           fDaclPresent;
	BOOL           fDaclDefaulted;
	ACL_SIZE_INFORMATION AclInfo;
	// һ���µ� ACL ����
	PACL           pNewACL = NULL;  //�ṹָ�����
	DWORD          cbNewACL = 0;     //ACL��size
									 // һ����ʱʹ�õ� ACE ����
	LPVOID         pTempAce = NULL;
	UINT           CurrentAceIndex = 0;  //ACE��ACL�е�λ��
	UINT           newAceIndex = 0;  //�����ACE��ACL�е�λ��
									 //API�����ķ���ֵ���������еĺ���������ʧ�ܡ�
	BOOL           fResult = FALSE;
	BOOL           fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
	// ����������������µ�API����������Windows 2000���ϰ汾�Ĳ���ϵͳ֧�֡� 
	// �ڴ˽���Advapi32.dll�ļ��ж�̬���롣�����ʹ��VC++ 6.0������򣬶�������
	// ʹ�������������ľ�̬���ӡ�����Ϊ��ı�����ϣ�/D_WIN32_WINNT=0x0500
	// �ı������������ȷ�����SDK��ͷ�ļ���lib�ļ������µġ�
	SetSecurityDescriptorControlFnPtr _SetSecurityDescriptorControl = NULL;
	AddAccessAllowedAceExFnPtr _AddAccessAllowedAceEx = NULL;
	do
	{
		// STEP 1: ͨ���û���ȡ��SID
		//     ����һ����LookupAccountName���������������Σ���һ����ȡ������Ҫ
		// ���ڴ�Ĵ�С��Ȼ�󣬽����ڴ���䡣�ڶ��ε��ò���ȡ�����û����ʻ���Ϣ��
		// LookupAccountNameͬ������ȡ�����û������û������Ϣ������ο�MSDN��
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		// ���ϵ���API��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
		// �����Ǵ�����ڴ治��Ĵ���
		if (fAPISuccess)
			break;
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			_tprintf(TEXT("LookupAccountName() failed. Error %d\n"), GetLastError());
			break;
		}
		pUserSID = myheapalloc(cbUserSID);
		if (!pUserSID)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		szDomain = (TCHAR *)myheapalloc(cbDomain * sizeof(TCHAR));
		if (!szDomain)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		if (!fAPISuccess)
		{
			_tprintf(TEXT("LookupAccountName() failed. Error %d\n"), GetLastError());
			break;
		}
		// 
		// STEP 2: ȡ���ļ���Ŀ¼����صİ�ȫ������SD
		//     ʹ��GetFileSecurity����ȡ��һ���ļ�SD�Ŀ�����ͬ�����������Ҳ
		// �Ǳ��������Σ���һ��ͬ����ȡSD���ڴ泤�ȡ�ע�⣬SD�����ָ�ʽ������ص�
		// ��self-relative���� ��ȫ�ģ�absolute����GetFileSecurityֻ��ȡ������
		// ��صġ�����SetFileSecurity����Ҫ��ȫ�ġ������Ϊʲô��Ҫһ���µ�SD��
		// ������ֱ����GetFileSecurity���ص�SD�Ͻ����޸ġ���Ϊ������صġ���Ϣ
		// �ǲ������ġ�
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, 0, &cbFileSD);
		// ���ϵ���API��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
		// �����Ǵ�����ڴ治��Ĵ���
		if (fAPISuccess)
			break;
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			_tprintf(TEXT("GetFileSecurity() failed. Error %d\n"), GetLastError());
			break;
		}
		pFileSD = myheapalloc(cbFileSD);
		if (!pFileSD)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		fAPISuccess = GetFileSecurity(lpszFileName,
			secInfo, pFileSD, cbFileSD, &cbFileSD);
		if (!fAPISuccess)
		{
			_tprintf(TEXT("GetFileSecurity() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 3: ��ʼ��һ���µ�SD
		if (!InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION))
		{
			_tprintf(TEXT("InitializeSecurityDescriptor() failed.")TEXT("Error %d\n"), GetLastError());
			break;
		}

		// STEP 4: ��GetFileSecurity ���ص�SD��ȡDACL 
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL, &fDaclDefaulted))
		{
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 5: ȡ DACL���ڴ�size
		//     GetAclInformation�����ṩDACL���ڴ��С��ֻ����һ������Ϊ
		// ACL_SIZE_INFORMATION��structure�Ĳ�������DACL����Ϣ����Ϊ��
		// �������Ǳ������е�ACE��
		AclInfo.AceCount = 0; // Assume NULL DACL.
		AclInfo.AclBytesFree = 0;
		AclInfo.AclBytesInUse = sizeof(ACL);
		if (pACL == NULL)
			fDaclPresent = FALSE;
		// ���DACL��Ϊ�գ���ȡ����Ϣ�������������¡��Թ�������DACLΪ�գ�
		if (fDaclPresent)
		{
			if (!GetAclInformation(pACL, &AclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
			{
				_tprintf(TEXT("GetAclInformation() failed. Error %d\n"), GetLastError());
				break;
			}
		}

		// STEP 6: �����µ�ACL��size
		//    ����Ĺ�ʽ�ǣ�ԭ�е�DACL��size������Ҫ��ӵ�һ��ACE��size����
		// ������һ����ACE��ص�SID��size������ȥ�����ֽ��Ի�þ�ȷ�Ĵ�С��
		cbNewACL = AclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE)
			+ GetLengthSid(pUserSID) - sizeof(DWORD);

		// STEP 7: Ϊ�µ�ACL�����ڴ�
		pNewACL = (PACL)myheapalloc(cbNewACL);
		if (!pNewACL)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 8: ��ʼ���µ�ACL�ṹ
		if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2))
		{
			_tprintf(TEXT("InitializeAcl() failed. Error %d\n"),
				GetLastError());
			break;
		}

		// STEP 9  ����ļ���Ŀ¼�� DACL �����ݣ��������е�ACE���µ�DACL��
		//     ����Ĵ���������ȼ��ָ���ļ���Ŀ¼���Ƿ���ڵ�DACL������еĻ���
		// ��ô�Ϳ������е�ACE���µ�DACL�ṹ�У����ǿ��Կ���������ķ����ǲ���
		// ACL_SIZE_INFORMATION�ṹ�е�AceCount��Ա����ɵġ������ѭ���У�
		// �ᰴ��Ĭ�ϵ�ACE��˳�������п�����ACE��ACL�е�˳���Ǻܹؼ��ģ����ڿ�
		// �������У��ȿ����Ǽ̳е�ACE������֪��ACE����ϲ�Ŀ¼�м̳�������
		newAceIndex = 0;
		if (fDaclPresent && AclInfo.AceCount)
		{
			bool bError = false;
			for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 10: ��DACL��ȡACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					bError = true;
					break;
				}
				// STEP 11: ����Ƿ��ǷǼ̳е�ACE
				//     �����ǰ��ACE��һ���Ӹ�Ŀ¼�̳�����ACE����ô���˳�ѭ����
				// ��Ϊ���̳е�ACE�����ڷǼ̳е�ACE֮�󣬶�������Ҫ��ӵ�ACE
				// Ӧ�������еķǼ̳е�ACE֮�����еļ̳е�ACE֮ǰ���˳�ѭ��
				// ����Ϊ��Ҫ���һ���µ�ACE���µ�DACL�У���������ٰѼ̳е�
				// ACE�������µ�DACL�С�
				if (((ACCESS_ALLOWED_ACE *)pTempAce)->Header.AceFlags& INHERITED_ACE)
					break;
				// STEP 12: ���Ҫ������ACE��SID�Ƿ����Ҫ�����ACE��SIDһ����
				// ���һ������ô��Ӧ�÷ϵ��Ѵ��ڵ�ACE��Ҳ����˵��ͬһ���û��Ĵ�ȡ
				// Ȩ�޵����õ�ACE����DACL��Ӧ��Ψһ�������������ͬһ�û�������
				// �˵�ACE�����ǿ��������û���ACE��
				if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE *)pTempAce)->SidStart)))
				{
					ACCESS_ALLOWED_ACE pTempAce2 = *(ACCESS_ALLOWED_ACE *)pTempAce;

					ACCESS_DENIED_ACE pTempAce4 = *(ACCESS_DENIED_ACE *)pTempAce;
					int a = -1;
					if (pTempAce2.Header.AceType == ACCESS_ALLOWED_ACE_TYPE)
					{
						a = 0;
					}
					else if (pTempAce2.Header.AceType == ACCESS_DENIED_ACE_TYPE)
					{
						a = 1;
					}
					else
						a = 2;
					continue;
				}
				// STEP 13: ��ACE���뵽�µ�DACL��
				//    ����Ĵ����У�ע�� AddAce �����ĵ����������������������˼�� 
				// ACL�е�����ֵ����ΪҪ��ACE�ӵ�ĳ����λ��֮�󣬲���MAXDWORD��
				// ��˼��ȷ����ǰ��ACE�Ǳ����뵽����λ�á�
				if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,
					((PACE_HEADER)pTempAce)->AceSize))
				{
					_tprintf(TEXT("AddAce() failed. Error %d\n"), GetLastError());
					break;
				}
				newAceIndex++;
			}
			if (bError)
			{
				break;
			}
		}

		// STEP 14: ��һ�� access-allowed ��ACE ���뵽�µ�DACL��
		//     ǰ���ѭ�����������еķǼ̳���SIDΪ�����û���ACE���˳�ѭ���ĵ�һ����
		// ���Ǽ�������ָ����ACE����ע�������ȶ�̬װ����һ��AddAccessAllowedAceEx
		// ��API���������װ�ز��ɹ����͵���AddAccessAllowedAce������ǰһ��������
		// ��Windows 2000�Ժ�İ汾֧�֣�NT��û�У�����Ϊ��ʹ���°汾�ĺ�����������
		// ���ȼ��һ�µ�ǰϵͳ�пɲ�����װ���������������������ʹ�á�ʹ�ö�̬����
		// ��ʹ�þ�̬���ӵĺô��ǣ���������ʱ������Ϊû�����API����������

		// Ex��ĺ��������һ������AceFlag�������˲�������������������ǿ���������һ
		// ����ACE_HEADER�Ľṹ���Ա������������õ�ACE���Ա�����Ŀ¼���̳���ȥ���� 
		// AddAccessAllowedAce�������ܶ��������������AddAccessAllowedAce����
		// �У�����ACE_HEADER����ṹ���óɷǼ̳еġ�
		_AddAccessAllowedAceEx = (AddAccessAllowedAceExFnPtr)
			GetProcAddress(GetModuleHandle(TEXT("advapi32.dll")), "AddAccessAllowedAceEx");
		if (_AddAccessAllowedAceEx)
		{
			if (!_AddAccessAllowedAceEx(pNewACL, ACL_REVISION2,
				CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE, dwAccessMask, pUserSID))
			{
				_tprintf(TEXT("AddAccessAllowedAceEx() failed. Error %d\n"), GetLastError());
				break;
			}
		}
		else
		{
			if (!AddAccessAllowedAce(pNewACL, ACL_REVISION2, dwAccessMask, pUserSID))
			{
				_tprintf(TEXT("AddAccessAllowedAce() failed. Error %d\n"), GetLastError());
				break;
			}
		}

		// STEP 15: �����Ѵ��ڵ�ACE��˳�򿽱��Ӹ�Ŀ¼�̳ж�����ACE
		bitset<32> bit(dwAccessMask);
		if (fDaclPresent && AclInfo.AceCount)
		{
			for (; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 16: ���ļ���Ŀ¼����DACL�м���ȡACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					break;
				}
				// STEP 17: ��ACE���뵽�µ�DACL��
				if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,
					((PACE_HEADER)pTempAce)->AceSize))
				{
					_tprintf(TEXT("AddAce() failed. Error %d\n"), GetLastError());
					break;
				}
			}
		}
		// STEP 18: ���µ�ACL���õ��µ�SD��
		if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL, FALSE))
		{
			_tprintf(TEXT("SetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 19: ���ϵ�SD�еĿ��Ʊ���ٿ������µ�SD�У�����ʹ�õ���һ���� 
		// SetSecurityDescriptorControl() ��API�������������ͬ��ֻ������
		// Windows 2000�Ժ�İ汾�У��������ǻ���Ҫ��̬�ذ����advapi32.dll 
		// �����룬���ϵͳ��֧������������ǾͲ������ϵ�SD�Ŀ��Ʊ���ˡ�
		_SetSecurityDescriptorControl = (SetSecurityDescriptorControlFnPtr)
			GetProcAddress(GetModuleHandle(TEXT("advapi32.dll")),
				"SetSecurityDescriptorControl");
		if (_SetSecurityDescriptorControl)
		{
			SECURITY_DESCRIPTOR_CONTROL controlBitsOfInterest = 0;
			SECURITY_DESCRIPTOR_CONTROL controlBitsToSet = 0;
			SECURITY_DESCRIPTOR_CONTROL oldControlBits = 0;
			DWORD dwRevision = 0;
			if (!GetSecurityDescriptorControl(pFileSD, &oldControlBits,
				&dwRevision))
			{
				_tprintf(TEXT("GetSecurityDescriptorControl() failed.")TEXT("Error %d\n"), GetLastError());
				break;
			}
			if (oldControlBits & SE_DACL_AUTO_INHERITED)
			{
				controlBitsOfInterest = SE_DACL_AUTO_INHERIT_REQ | SE_DACL_AUTO_INHERITED;
				controlBitsToSet = controlBitsOfInterest;
			}
			else if (oldControlBits & SE_DACL_PROTECTED)
			{
				controlBitsOfInterest = SE_DACL_PROTECTED;
				controlBitsToSet = controlBitsOfInterest;
			}
			if (controlBitsOfInterest)
			{
				if (!_SetSecurityDescriptorControl(&newSD, controlBitsOfInterest, controlBitsToSet))
				{
					_tprintf(TEXT("SetSecurityDescriptorControl() failed.")TEXT("Error %d\n"), GetLastError());
					break;
				}
			}
		}

		// STEP 20: ���µ�SD�������õ��ļ��İ�ȫ�����У�ǧɽ��ˮ�������ڵ��ˣ�
		if (!SetFileSecurity(lpszFileName, secInfo, &newSD))
		{
			_tprintf(TEXT("SetFileSecurity() failed. Error %d\n"),
				GetLastError());
			break;
		}
		fResult = TRUE;
	} while (0);
	if (pUserSID)  myheapfree(pUserSID);
	if (szDomain)  myheapfree(szDomain);
	if (pFileSD) myheapfree(pFileSD);
	if (pNewACL) myheapfree(pNewACL);

	return fResult;
}


//     ��ȡ���ļ�(Ŀ¼)ָ���ʻ����飩��Ȩ��
//     TCHAR *lpszFileName     �ļ���Ŀ¼��
//     TCHAR *lpszAccountName  �ʻ����飩
//       int (&arrRights)[32]       �������ã�Ҫ�������������32��int����
BOOL FileAttrHelperEx::GetAccountRights(TCHAR *lpszFileName, TCHAR *lpszAccountName, int(&arrRights)[32])
{
	//������arrRights��ʼ��Ϊ0
	for (int i = 0; i < 32; i++)
	{
		arrRights[i] = 0;
	}
	// ����SID����
	SID_NAME_USE   snuType;

	// ������LookupAccountName��صı�����ע�⣬ȫΪ0��Ҫ�ڳ����ж�̬���䣩
	TCHAR *        szDomain = NULL;
	DWORD          cbDomain = 0;
	LPVOID         pUserSID = NULL;
	DWORD          cbUserSID = 0;

	// ���ļ���صİ�ȫ������ SD �ı���
	PSECURITY_DESCRIPTOR pFileSD = NULL;								// �ṹ����
	DWORD          cbFileSD = 0;													// SD��size

																					// ��ACL ��صı���
	PACL           pACL = NULL;
	BOOL           fDaclPresent;
	BOOL           fDaclDefaulted;
	ACL_SIZE_INFORMATION AclInfo;

	// һ����ʱʹ�õ� ACE ����
	LPVOID         pTempAce = NULL;
	UINT           CurrentAceIndex = 0;

	//ACE��ACL�е�λ��
	BOOL           fResult = FALSE;
	BOOL           fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
	do
	{
		// STEP 1: ͨ���û���ȡ��SID
		//     ����һ����LookupAccountName���������������Σ���һ����ȡ������Ҫ
		// ���ڴ�Ĵ�С��Ȼ�󣬽����ڴ���䡣�ڶ��ε��ò���ȡ�����û����ʻ���Ϣ��
		// LookupAccountNameͬ������ȡ�����û������û������Ϣ������ο�MSDN��
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		// ���ϵ���API��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
		// �����Ǵ�����ڴ治��Ĵ���
		if (fAPISuccess)
			break;
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			_tprintf(TEXT("LookupAccountName() failed. Error %d\n"), GetLastError());
			break;
		}
		pUserSID = myheapalloc(cbUserSID);
		if (!pUserSID)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		szDomain = (TCHAR *)myheapalloc(cbDomain * sizeof(TCHAR));
		if (!szDomain)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		if (!fAPISuccess)
		{
			_tprintf(TEXT("LookupAccountName() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 2: ȡ���ļ���Ŀ¼����صİ�ȫ������SD
		//     ʹ��GetFileSecurity����ȡ��һ���ļ�SD�Ŀ�����ͬ�����������Ҳ
		// �Ǳ��������Σ���һ��ͬ����ȡSD���ڴ泤�ȡ�ע�⣬SD�����ָ�ʽ������ص�
		// ��self-relative���� ��ȫ�ģ�absolute����GetFileSecurityֻ��ȡ��������صġ���
		//��SetFileSecurity����Ҫ��ȫ�ġ������Ϊʲô��Ҫһ���µ�SD��
		// ������ֱ����GetFileSecurity���ص�SD�Ͻ����޸ġ���Ϊ������صġ���Ϣ�ǲ������ġ�
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, 0, &cbFileSD);
		// ���ϵ���API��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
		// �����Ǵ�����ڴ治��Ĵ���
		if (fAPISuccess)
			break;
		else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			_tprintf(TEXT("GetFileSecurity() failed. Error %d\n"), GetLastError());
			break;
		}
		pFileSD = myheapalloc(cbFileSD);
		if (!pFileSD)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, cbFileSD, &cbFileSD);
		if (!fAPISuccess)
		{
			_tprintf(TEXT("GetFileSecurity() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 3: ��GetFileSecurity ���ص�SD��ȡDACL
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,
			&fDaclDefaulted))
		{
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 4: ȡ DACL���ڴ�size
		//     GetAclInformation�����ṩDACL���ڴ��С��ֻ����һ������Ϊ
		// ACL_SIZE_INFORMATION��structure�Ĳ�������DACL����Ϣ����Ϊ��
		// �������Ǳ������е�ACE��
		AclInfo.AceCount = 0; // Assume NULL DACL.
		AclInfo.AclBytesFree = 0;
		AclInfo.AclBytesInUse = sizeof(ACL);
		if (pACL == NULL)
			fDaclPresent = FALSE;
		// ���DACL��Ϊ�գ���ȡ����Ϣ�������������¡��Թ�������DACLΪ�գ�
		if (fDaclPresent)
		{
			if (!GetAclInformation(pACL, &AclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
			{
				_tprintf(TEXT("GetAclInformation() failed. Error %d\n"), GetLastError());
				break;
			}
		}

		// STEP 5  ����ļ���Ŀ¼�� DACL �����ݣ���ָ���ʻ���ACE�ķ���Ȩ��ת������������
		//     ����Ĵ���������ȼ��ָ���ļ���Ŀ¼���Ƿ���ڵ�DACL������еĻ���
		// ��ô�ͽ�ָ���ʻ���ACE�ķ���Ȩ��ת�����������飬���ǿ��Կ���������ķ���
		// �ǲ���ACL_SIZE_INFORMATION�ṹ�е�AceCount��Ա����ɵġ������ѭ���У�
		// ���Һ�ָ���˻���ص�ACE
		if (fDaclPresent && AclInfo.AceCount)
		{
			for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 10: ��DACL��ȡACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					break;
				}
				// STEP 6: ���Ҫ������ACE��SID�Ƿ����Ҫ�����ACE��SIDһ����
				// ���һ������ô�ͽ���ACE�ķ���Ȩ��ת�����������飬����������������һ��ѭ��
				int nAceType = 1;
				if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE *)pTempAce)->SidStart)))
				{
					if (((PACE_HEADER)pTempAce)->AceType == ACCESS_DENIED_ACE_TYPE)
					{
						nAceType = 2;
					}
					else
					{
						nAceType = 1;
					}
					//bitset������������ֵ��˳���Ǵ�0��N-1
					bitset<32> bitAccessMask(((ACCESS_ALLOWED_ACE*)pTempAce)->Mask);
					for (int i = 0; i < 32; i++)
					{
						if (bitAccessMask[i] != 0 && arrRights[i] != 2)
						{
							arrRights[i] = nAceType;
						}
					}
				}
				else
				{
					continue;
				}
			}
		}
		fResult = TRUE;
	} while (0);
	if (pUserSID)  myheapfree(pUserSID);
	if (szDomain)  myheapfree(szDomain);
	if (pFileSD) myheapfree(pFileSD);
	return fResult;
}

BOOL FileAttrHelperEx::AltEnableFileAccountPrivilege(PCTSTR pszPath, PCTSTR pszAccount, DWORD dwAccessMask)
{
	CDacl dacl;
	CSid sid;
	// ��ȡ�û��ʻ���־��
	if (!sid.LoadAccount(pszAccount))
	{
		return FALSE;
	}
	// ��ȡ�ļ�(��)��DACL
	if (!AtlGetDacl(pszPath, SE_FILE_OBJECT, &dacl))
	{
		return FALSE;
	}
	// ��DACL������µ�ACE��
	if (!dacl.AddAllowedAce(sid, dwAccessMask))
	{
		return FALSE;
	}
	// �����ļ�(��)��DACL
	return AtlSetDacl(pszPath, SE_FILE_OBJECT, dacl);
}

DWORD FileAttrHelperEx::SetFolderShare(const WCHAR* strFolderPath, const WCHAR* strShareName,
	const TCHAR* strUserName, DWORD dwAccessMask)
{
	DWORD parmErr = 0;
	DWORD level = 502;
	WCHAR folderPath[MAX_PATH] = { 0 };
	wcscpy(folderPath, strFolderPath);
	WCHAR shareName[MAX_PATH] = { 0 };
	wcscpy(shareName, strShareName);

	SHARE_INFO_502 si502;
	si502.shi502_netname = shareName;							// ������
	si502.shi502_type = STYPE_DISKTREE;                             // ��Դ���� �����Ǵ���
	si502.shi502_remark = NULL;                                     // ע��
	si502.shi502_path = folderPath;										// �ļ���·��
	si502.shi502_permissions = ACCESS_ALL;                          // ����Ȩ��
	si502.shi502_passwd = NULL;                                     // ��������
	si502.shi502_max_uses = -1;                                     // ����û�����
	si502.shi502_current_uses = 0;                                  // ��ǰ�����û�
	si502.shi502_reserved = 0;										// �����ֶ�

	SECURITY_DESCRIPTOR sd;   //��������һ��SD
	BYTE aclBuffer[1024] = { 0 };

	PACL pacl = (PACL)&aclBuffer;  //����һ��ACL��������1024
								   //��ʼ��һ��SD
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	PSID psid;
	//��ʼ��һ��ACL
	InitializeAcl(pacl, 1024, ACL_REVISION);

	if (!SetAce(strUserName, pacl, dwAccessMask))
	{
		return GetLastError();
	}

	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);							//��ACL���õ�SD��
	si502.shi502_security_descriptor = &sd;												//����ɵ�SD�ṹ��ֵ��502�ṹ��  

	NET_API_STATUS status;
	DWORD paraerr = 0;
	status = NetShareAdd(NULL, 502, (LPBYTE)&si502, &paraerr);

	if (status != NERR_Success)
	{
		if (status == NERR_DuplicateShare)
		{
			NetShareDel(NULL, shareName, 0);
			if (NetShareAdd(NULL, 502, (LPBYTE)&si502, &paraerr) == NERR_Success)
			{
				return 0;
			}
		}
		return status;
	}
	return 0;
}

DWORD FileAttrHelperEx::SetFolderPrivate(const WCHAR* strShareName)
{
	WCHAR shareName[MAX_PATH] = { 0 };
	wcscpy(shareName, strShareName);
	return NetShareDel(NULL, shareName, 0);
}
BOOL  FileAttrHelperEx::SetAce(const stdstr& strAccountName, PACL pacl, DWORD accessMask)
{
	PSID pSID = NULL;
	DWORD cbSid = 0;
	LPTSTR DomainName = NULL;
	DWORD cbDomainName = 0;
	SID_NAME_USE SIDNameUse;
	BOOL  bDone = TRUE;
	do
	{
		if (!LookupAccountName(NULL, strAccountName.c_str(), pSID, &cbSid, DomainName, &cbDomainName, &SIDNameUse))
		{
			pSID = (PSID)malloc(cbSid);
			DomainName = (LPTSTR)malloc(cbDomainName * sizeof(TCHAR));
			if (!pSID || !DomainName)
			{
				bDone = FALSE;
				break;
			}

			if (!LookupAccountName(NULL, strAccountName.c_str(), pSID, &cbSid, DomainName, &cbDomainName, &SIDNameUse))
			{
				bDone = FALSE;
				break;
			}
			bDone = AddAccessAllowedAce(pacl, ACL_REVISION, accessMask, pSID);
		}
	} while (0);
	if (DomainName)
	{
		free(DomainName);
		DomainName = NULL;
	}
	if (pSID)
	{
		free(pSID);
		pSID = NULL;
	}
	return bDone;
}
