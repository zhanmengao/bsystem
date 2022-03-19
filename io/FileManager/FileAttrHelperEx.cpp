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

//使用Windows的HeapAlloc函数进行动态内存分配
#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

typedef BOOL(WINAPI *SetSecurityDescriptorControlFnPtr)(
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);
typedef BOOL(WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl, DWORD dwAceRevision,
	DWORD AceFlags, DWORD AccessMask, PSID pSid);

//     为文件(目录)添加一个帐户（组）的权限
//     TCHAR *lpszFileName     文件（目录）
//     TCHAR *lpszAccountName  帐户（组）
//       DWORD dwAccessMask       权限设置(如GENERIC_ALL，GENERIC_READ等)
BOOL FileAttrHelperEx::EnableFileAccountPrivilege(TCHAR *lpszFileName, TCHAR *lpszAccountName, DWORD dwAccessMask)
{
	// 声明SID变量
	SID_NAME_USE   snuType;
	// 声明和LookupAccountName相关的变量（注意，全为0，要在程序中动态分配）
	TCHAR *        szDomain = NULL;
	DWORD          cbDomain = 0;
	LPVOID         pUserSID = NULL;
	DWORD          cbUserSID = 0;
	// 和文件相关的安全描述符 SD 的变量
	PSECURITY_DESCRIPTOR pFileSD = NULL;     // 结构变量
	DWORD          cbFileSD = 0;

	// 一个新的SD的变量，用于构造新的ACL（把已有的ACL和需要新加的ACL整合起来）
	SECURITY_DESCRIPTOR  newSD;
	// 和ACL 相关的变量
	PACL           pACL = NULL;
	BOOL           fDaclPresent;
	BOOL           fDaclDefaulted;
	ACL_SIZE_INFORMATION AclInfo;
	// 一个新的 ACL 变量
	PACL           pNewACL = NULL;  //结构指针变量
	DWORD          cbNewACL = 0;     //ACL的size
									 // 一个临时使用的 ACE 变量
	LPVOID         pTempAce = NULL;
	UINT           CurrentAceIndex = 0;  //ACE在ACL中的位置
	UINT           newAceIndex = 0;  //新添的ACE在ACL中的位置
									 //API函数的返回值，假设所有的函数都返回失败。
	BOOL           fResult = FALSE;
	BOOL           fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
	// 下面的两个函数是新的API函数，仅在Windows 2000以上版本的操作系统支持。 
	// 在此将从Advapi32.dll文件中动态载入。如果你使用VC++ 6.0编译程序，而且你想
	// 使用这两个函数的静态链接。则请为你的编译加上：/D_WIN32_WINNT=0x0500
	// 的编译参数。并且确保你的SDK的头文件和lib文件是最新的。
	SetSecurityDescriptorControlFnPtr _SetSecurityDescriptorControl = NULL;
	AddAccessAllowedAceExFnPtr _AddAccessAllowedAceEx = NULL;
	do
	{
		// STEP 1: 通过用户名取得SID
		//     在这一步中LookupAccountName函数被调用了两次，第一次是取出所需要
		// 的内存的大小，然后，进行内存分配。第二次调用才是取得了用户的帐户信息。
		// LookupAccountName同样可以取得域用户或是用户组的信息。（请参看MSDN）
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		// 以上调用API会失败，失败原因是内存不足。并把所需要的内存大小传出。
		// 下面是处理非内存不足的错误。
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
		// STEP 2: 取得文件（目录）相关的安全描述符SD
		//     使用GetFileSecurity函数取得一份文件SD的拷贝，同样，这个函数也
		// 是被调用两次，第一次同样是取SD的内存长度。注意，SD有两种格式：自相关的
		// （self-relative）和 完全的（absolute），GetFileSecurity只能取到“自
		// 相关的”，而SetFileSecurity则需要完全的。这就是为什么需要一个新的SD，
		// 而不是直接在GetFileSecurity返回的SD上进行修改。因为“自相关的”信息
		// 是不完整的。
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, 0, &cbFileSD);
		// 以上调用API会失败，失败原因是内存不足。并把所需要的内存大小传出。
		// 下面是处理非内存不足的错误。
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

		// STEP 3: 初始化一个新的SD
		if (!InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION))
		{
			_tprintf(TEXT("InitializeSecurityDescriptor() failed.")TEXT("Error %d\n"), GetLastError());
			break;
		}

		// STEP 4: 从GetFileSecurity 返回的SD中取DACL 
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL, &fDaclDefaulted))
		{
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 5: 取 DACL的内存size
		//     GetAclInformation可以提供DACL的内存大小。只传入一个类型为
		// ACL_SIZE_INFORMATION的structure的参数，需DACL的信息，是为了
		// 方便我们遍历其中的ACE。
		AclInfo.AceCount = 0; // Assume NULL DACL.
		AclInfo.AclBytesFree = 0;
		AclInfo.AclBytesInUse = sizeof(ACL);
		if (pACL == NULL)
			fDaclPresent = FALSE;
		// 如果DACL不为空，则取其信息。（大多数情况下“自关联”的DACL为空）
		if (fDaclPresent)
		{
			if (!GetAclInformation(pACL, &AclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
			{
				_tprintf(TEXT("GetAclInformation() failed. Error %d\n"), GetLastError());
				break;
			}
		}

		// STEP 6: 计算新的ACL的size
		//    计算的公式是：原有的DACL的size加上需要添加的一个ACE的size，以
		// 及加上一个和ACE相关的SID的size，最后减去两个字节以获得精确的大小。
		cbNewACL = AclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE)
			+ GetLengthSid(pUserSID) - sizeof(DWORD);

		// STEP 7: 为新的ACL分配内存
		pNewACL = (PACL)myheapalloc(cbNewACL);
		if (!pNewACL)
		{
			_tprintf(TEXT("HeapAlloc() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 8: 初始化新的ACL结构
		if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2))
		{
			_tprintf(TEXT("InitializeAcl() failed. Error %d\n"),
				GetLastError());
			break;
		}

		// STEP 9  如果文件（目录） DACL 有数据，拷贝其中的ACE到新的DACL中
		//     下面的代码假设首先检查指定文件（目录）是否存在的DACL，如果有的话，
		// 那么就拷贝所有的ACE到新的DACL结构中，我们可以看到其遍历的方法是采用
		// ACL_SIZE_INFORMATION结构中的AceCount成员来完成的。在这个循环中，
		// 会按照默认的ACE的顺序来进行拷贝（ACE在ACL中的顺序是很关键的），在拷
		// 贝过程中，先拷贝非继承的ACE（我们知道ACE会从上层目录中继承下来）
		newAceIndex = 0;
		if (fDaclPresent && AclInfo.AceCount)
		{
			bool bError = false;
			for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 10: 从DACL中取ACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					bError = true;
					break;
				}
				// STEP 11: 检查是否是非继承的ACE
				//     如果当前的ACE是一个从父目录继承来的ACE，那么就退出循环。
				// 因为，继承的ACE总是在非继承的ACE之后，而我们所要添加的ACE
				// 应该在已有的非继承的ACE之后，所有的继承的ACE之前。退出循环
				// 正是为了要添加一个新的ACE到新的DACL中，这后，我们再把继承的
				// ACE拷贝到新的DACL中。
				if (((ACCESS_ALLOWED_ACE *)pTempAce)->Header.AceFlags& INHERITED_ACE)
					break;
				// STEP 12: 检查要拷贝的ACE的SID是否和需要加入的ACE的SID一样，
				// 如果一样，那么就应该废掉已存在的ACE，也就是说，同一个用户的存取
				// 权限的设置的ACE，在DACL中应该唯一。这在里，跳过对同一用户已设置
				// 了的ACE，仅是拷贝其它用户的ACE。
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
				// STEP 13: 把ACE加入到新的DACL中
				//    下面的代码中，注意 AddAce 函数的第三个参数，这个参数的意思是 
				// ACL中的索引值，意为要把ACE加到某索引位置之后，参数MAXDWORD的
				// 意思是确保当前的ACE是被加入到最后的位置。
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

		// STEP 14: 把一个 access-allowed 的ACE 加入到新的DACL中
		//     前面的循环拷贝了所有的非继承且SID为其它用户的ACE，退出循环的第一件事
		// 就是加入我们指定的ACE。请注意首先先动态装载了一个AddAccessAllowedAceEx
		// 的API函数，如果装载不成功，就调用AddAccessAllowedAce函数。前一个函数仅
		// 在Windows 2000以后的版本支持，NT则没有，我们为了使用新版本的函数，我们首
		// 先先检查一下当前系统中可不可以装载这个函数，如果可以则就使用。使用动态链接
		// 比使用静态链接的好处是，程序运行时不会因为没有这个API函数而报错。

		// Ex版的函数多出了一个参数AceFlag（第三人参数），用这个参数我们可以来设置一
		// 个叫ACE_HEADER的结构，以便让我们所设置的ACE可以被其子目录所继承下去，而 
		// AddAccessAllowedAce函数不能定制这个参数，在AddAccessAllowedAce函数
		// 中，其会把ACE_HEADER这个结构设置成非继承的。
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

		// STEP 15: 按照已存在的ACE的顺序拷贝从父目录继承而来的ACE
		bitset<32> bit(dwAccessMask);
		if (fDaclPresent && AclInfo.AceCount)
		{
			for (; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 16: 从文件（目录）的DACL中继续取ACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					break;
				}
				// STEP 17: 把ACE加入到新的DACL中
				if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,
					((PACE_HEADER)pTempAce)->AceSize))
				{
					_tprintf(TEXT("AddAce() failed. Error %d\n"), GetLastError());
					break;
				}
			}
		}
		// STEP 18: 把新的ACL设置到新的SD中
		if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL, FALSE))
		{
			_tprintf(TEXT("SetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 19: 把老的SD中的控制标记再拷贝到新的SD中，我们使用的是一个叫 
		// SetSecurityDescriptorControl() 的API函数，这个函数同样只存在于
		// Windows 2000以后的版本中，所以我们还是要动态地把其从advapi32.dll 
		// 中载入，如果系统不支持这个函数，那就不拷贝老的SD的控制标记了。
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

		// STEP 20: 把新的SD设置设置到文件的安全属性中（千山万水啊，终于到了）
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


//     获取该文件(目录)指定帐户（组）的权限
//     TCHAR *lpszFileName     文件（目录）
//     TCHAR *lpszAccountName  帐户（组）
//       int (&arrRights)[32]       数组引用，要求传入参数必须是32个int数组
BOOL FileAttrHelperEx::GetAccountRights(TCHAR *lpszFileName, TCHAR *lpszAccountName, int(&arrRights)[32])
{
	//将参数arrRights初始化为0
	for (int i = 0; i < 32; i++)
	{
		arrRights[i] = 0;
	}
	// 声明SID变量
	SID_NAME_USE   snuType;

	// 声明和LookupAccountName相关的变量（注意，全为0，要在程序中动态分配）
	TCHAR *        szDomain = NULL;
	DWORD          cbDomain = 0;
	LPVOID         pUserSID = NULL;
	DWORD          cbUserSID = 0;

	// 和文件相关的安全描述符 SD 的变量
	PSECURITY_DESCRIPTOR pFileSD = NULL;								// 结构变量
	DWORD          cbFileSD = 0;													// SD的size

																					// 和ACL 相关的变量
	PACL           pACL = NULL;
	BOOL           fDaclPresent;
	BOOL           fDaclDefaulted;
	ACL_SIZE_INFORMATION AclInfo;

	// 一个临时使用的 ACE 变量
	LPVOID         pTempAce = NULL;
	UINT           CurrentAceIndex = 0;

	//ACE在ACL中的位置
	BOOL           fResult = FALSE;
	BOOL           fAPISuccess = FALSE;
	SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
	do
	{
		// STEP 1: 通过用户名取得SID
		//     在这一步中LookupAccountName函数被调用了两次，第一次是取出所需要
		// 的内存的大小，然后，进行内存分配。第二次调用才是取得了用户的帐户信息。
		// LookupAccountName同样可以取得域用户或是用户组的信息。（请参看MSDN）
		fAPISuccess = LookupAccountName(NULL, lpszAccountName,
			pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
		// 以上调用API会失败，失败原因是内存不足。并把所需要的内存大小传出。
		// 下面是处理非内存不足的错误。
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

		// STEP 2: 取得文件（目录）相关的安全描述符SD
		//     使用GetFileSecurity函数取得一份文件SD的拷贝，同样，这个函数也
		// 是被调用两次，第一次同样是取SD的内存长度。注意，SD有两种格式：自相关的
		// （self-relative）和 完全的（absolute），GetFileSecurity只能取到“自相关的”，
		//而SetFileSecurity则需要完全的。这就是为什么需要一个新的SD，
		// 而不是直接在GetFileSecurity返回的SD上进行修改。因为“自相关的”信息是不完整的。
		fAPISuccess = GetFileSecurity(lpszFileName, secInfo, pFileSD, 0, &cbFileSD);
		// 以上调用API会失败，失败原因是内存不足。并把所需要的内存大小传出。
		// 下面是处理非内存不足的错误。
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

		// STEP 3: 从GetFileSecurity 返回的SD中取DACL
		if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,
			&fDaclDefaulted))
		{
			_tprintf(TEXT("GetSecurityDescriptorDacl() failed. Error %d\n"), GetLastError());
			break;
		}

		// STEP 4: 取 DACL的内存size
		//     GetAclInformation可以提供DACL的内存大小。只传入一个类型为
		// ACL_SIZE_INFORMATION的structure的参数，需DACL的信息，是为了
		// 方便我们遍历其中的ACE。
		AclInfo.AceCount = 0; // Assume NULL DACL.
		AclInfo.AclBytesFree = 0;
		AclInfo.AclBytesInUse = sizeof(ACL);
		if (pACL == NULL)
			fDaclPresent = FALSE;
		// 如果DACL不为空，则取其信息。（大多数情况下“自关联”的DACL为空）
		if (fDaclPresent)
		{
			if (!GetAclInformation(pACL, &AclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
			{
				_tprintf(TEXT("GetAclInformation() failed. Error %d\n"), GetLastError());
				break;
			}
		}

		// STEP 5  如果文件（目录） DACL 有数据，将指定帐户的ACE的访问权限转换到整型数组
		//     下面的代码假设首先检查指定文件（目录）是否存在的DACL，如果有的话，
		// 那么就将指定帐户的ACE的访问权限转换到整型数组，我们可以看到其遍历的方法
		// 是采用ACL_SIZE_INFORMATION结构中的AceCount成员来完成的。在这个循环中，
		// 查找和指定账户相关的ACE
		if (fDaclPresent && AclInfo.AceCount)
		{
			for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
			{
				// STEP 10: 从DACL中取ACE
				if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
				{
					_tprintf(TEXT("GetAce() failed. Error %d\n"), GetLastError());
					break;
				}
				// STEP 6: 检查要拷贝的ACE的SID是否和需要加入的ACE的SID一样，
				// 如果一样，那么就将该ACE的访问权限转换到整型数组，否则跳过，进行下一个循环
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
					//bitset类代表的整型数值的顺序是从0到N-1
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
	// 获取用户帐户标志符
	if (!sid.LoadAccount(pszAccount))
	{
		return FALSE;
	}
	// 获取文件(夹)的DACL
	if (!AtlGetDacl(pszPath, SE_FILE_OBJECT, &dacl))
	{
		return FALSE;
	}
	// 在DACL中添加新的ACE项
	if (!dacl.AddAllowedAce(sid, dwAccessMask))
	{
		return FALSE;
	}
	// 设置文件(夹)的DACL
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
	si502.shi502_netname = shareName;							// 共享名
	si502.shi502_type = STYPE_DISKTREE;                             // 资源类型 这里是磁盘
	si502.shi502_remark = NULL;                                     // 注释
	si502.shi502_path = folderPath;										// 文件夹路径
	si502.shi502_permissions = ACCESS_ALL;                          // 访问权限
	si502.shi502_passwd = NULL;                                     // 访问密码
	si502.shi502_max_uses = -1;                                     // 最大用户连接
	si502.shi502_current_uses = 0;                                  // 当前连接用户
	si502.shi502_reserved = 0;										// 保留字段

	SECURITY_DESCRIPTOR sd;   //首先声明一个SD
	BYTE aclBuffer[1024] = { 0 };

	PACL pacl = (PACL)&aclBuffer;  //声明一个ACL，长度是1024
								   //初始化一个SD
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	PSID psid;
	//初始化一个ACL
	InitializeAcl(pacl, 1024, ACL_REVISION);

	if (!SetAce(strUserName, pacl, dwAccessMask))
	{
		return GetLastError();
	}

	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);							//把ACL设置到SD中
	si502.shi502_security_descriptor = &sd;												//将完成的SD结构赋值到502结构里  

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
