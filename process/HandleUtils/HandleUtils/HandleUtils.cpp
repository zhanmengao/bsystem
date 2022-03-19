#include"HandleUtils.h"

HANDLE HandleUtils::CopyHandle(HANDLE hSrcProcess, HANDLE hTargetProcess, HANDLE handle,
	DWORD access, BOOL bInherit, DWORD option)
{
	HANDLE hObjInProcessT = NULL;
	DuplicateHandle(hSrcProcess, handle, hTargetProcess, &hObjInProcessT, access, bInherit, option);
	return hObjInProcessT;
}
DWORD HandleUtils::GetHandleFlags(HANDLE handle)
{
	DWORD flags;
	if (GetHandleInformation(handle, &flags))
	{
		return flags;
	}
	return 0;
}
BOOL HandleUtils::SetHandleInherit(HANDLE handle, bool bInherit)
{
	return SetHandleInformation(handle, HANDLE_FLAG_INHERIT, bInherit);
}
bool HandleUtils::GetHandleInherit(HANDLE handle)
{
	return (0 != GetHandleFlags(handle) & HANDLE_FLAG_INHERIT);
}
BOOL HandleUtils::SetHandleProtectFromClose(HANDLE handle, bool bInherit)
{
	return SetHandleInformation(handle, HANDLE_FLAG_PROTECT_FROM_CLOSE, bInherit);
}
void HandleUtils::MakeSa(SECURITY_ATTRIBUTES& sa,bool bInheritHandle)
{
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = bInheritHandle;
}
SECURITY_ATTRIBUTES HandleUtils::GetInheritSa()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	return sa;
}

HANDLE HandleUtils::dup2(HANDLE hSrcProcess, HANDLE hSrcHandle, HANDLE hTargetProcess)
{
	HANDLE hTargerHandle;
	DuplicateHandle(hSrcProcess, hSrcHandle, hTargetProcess, &hTargerHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	return hTargerHandle;
}