#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>

int main3()
{
	LPGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	NET_API_STATUS nStatus;
	//
	// Call the NetUserGetGroups function, specifying level 0.
	//
	nStatus = NetUserGetGroups(NULL,
		L"zzz",
		dwLevel,
		(LPBYTE*)&pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries);
	//
	// If the call succeeds,
	//
	if (nStatus == NERR_Success)
	{
		LPGROUP_USERS_INFO_0 pTmpBuf;
		DWORD i;
		DWORD dwTotalCount = 0;

		if ((pTmpBuf = pBuf) != NULL)
		{
			fprintf(stderr, "\nGlobal group(s):\n");
			//
			// Loop through the entries; 
			//  print the name of the global groups 
			//  to which the user belongs.
			//
			for (i = 0; i < dwEntriesRead; i++)
			{
				assert(pTmpBuf != NULL);

				if (pTmpBuf == NULL)
				{
					fprintf(stderr, "An access violation has occurred\n");
					break;
				}

				wprintf(L"\t-- %s\n", pTmpBuf->grui0_name);

				pTmpBuf++;
				dwTotalCount++;
			}
		}
		//
		// If all available entries were
		//  not enumerated, print the number actually 
		//  enumerated and the total number available.
		//
		if (dwEntriesRead < dwTotalEntries)
			fprintf(stderr, "\nTotal entries: %d", dwTotalEntries);
		//
		// Otherwise, just print the total.
		//
		printf("\nEntries enumerated: %d\n", dwTotalCount);
	}
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	//
	// Free the allocated buffer.
	//
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}