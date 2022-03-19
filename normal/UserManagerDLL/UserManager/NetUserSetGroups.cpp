#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>

int wmain4(int argc, wchar_t *argv[])
{
	DWORD dwLevel = 0;
	GROUP_USERS_INFO_0 gi;
	NET_API_STATUS nStatus;
	// Fill in the GROUP_USERS_INFO_0 structure member.
	gi.grui0_name = argv[3];
	// Call the NetUserSetGroups function; specify level 0.
	nStatus = NetUserSetGroups(argv[1],
		argv[2],
		dwLevel,
		(LPBYTE)&gi,
		1);
	// If the call succeeds, inform the user.
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"Group membership has been successful for %s\n", argv[2]);
	// Otherwise, print the system error.
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return 0;
}