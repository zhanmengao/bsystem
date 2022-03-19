#include <iostream>
using namespace std;
#include <Wbemidl.h>

# pragma comment(lib, "wbemuuid.lib")

int __cdecl wmainmmm(int argc, wchar_t* argv[])
{
	// To add error checking,
	// check returned HRESULT below where collected.
	HRESULT                 hr = S_OK;
	IWbemRefresher          *pRefresher = NULL;
	IWbemConfigureRefresher *pConfig = NULL;
	IWbemHiPerfEnum         *pEnum = NULL;
	IWbemServices           *pNameSpace = NULL;
	IWbemLocator            *pWbemLocator = NULL;
	IWbemObjectAccess       **apEnumAccess = NULL;
	BSTR                    bstrNameSpace = NULL;
	long                    lID = 0;
	long                    lVirtualBytesHandle = 0;
	long                    lIDProcessHandle = 0;
	DWORD                   dwVirtualBytes = 0;
	DWORD                   dwProcessId = 0;
	DWORD                   dwNumObjects = 0;
	DWORD                   dwNumReturned = 0;
	DWORD                   dwIDProcess = 0;
	DWORD                   i = 0;
	int                     x = 0;

	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, 0)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = CoCreateInstance(
		CLSID_WbemLocator,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(void**)&pWbemLocator)))
	{
		goto CLEANUP;
	}

	// Connect to the desired namespace.
	bstrNameSpace = SysAllocString(L"\\\\.\\root\\cimv2");
	if (NULL == bstrNameSpace)
	{
		hr = E_OUTOFMEMORY;
		goto CLEANUP;
	}
	if (FAILED(hr = pWbemLocator->ConnectServer(
		bstrNameSpace,
		NULL, // User name
		NULL, // Password
		NULL, // Locale
		0L,   // Security flags
		NULL, // Authority
		NULL, // Wbem context
		&pNameSpace)))
	{
		goto CLEANUP;
	}
	pWbemLocator->Release();
	pWbemLocator = NULL;
	SysFreeString(bstrNameSpace);
	bstrNameSpace = NULL;

	if (FAILED(hr = CoCreateInstance(
		CLSID_WbemRefresher,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemRefresher,
		(void**)&pRefresher)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = pRefresher->QueryInterface(
		IID_IWbemConfigureRefresher,
		(void **)&pConfig)))
	{
		goto CLEANUP;
	}

	// Add an enumerator to the refresher.
	if (FAILED(hr = pConfig->AddEnum(
		pNameSpace,
		L"Win32_PerfRawData_PerfProc_Process",
		0,
		NULL,
		&pEnum,
		&lID)))
	{
		goto CLEANUP;
	}
	pConfig->Release();
	pConfig = NULL;

	// Get a property handle for the VirtualBytes property.

	// Refresh the object ten times and retrieve the value.
	for (x = 0; x < 10; x++)
	{
		dwNumReturned = 0;
		dwIDProcess = 0;
		dwNumObjects = 0;

		if (FAILED(hr = pRefresher->Refresh(0L)))
		{
			goto CLEANUP;
		}

		hr = pEnum->GetObjects(0L,
			dwNumObjects,
			apEnumAccess,
			&dwNumReturned);
		// If the buffer was not big enough,
		// allocate a bigger buffer and retry.
		if (hr == WBEM_E_BUFFER_TOO_SMALL
			&& dwNumReturned > dwNumObjects)
		{
			apEnumAccess = new IWbemObjectAccess*[dwNumReturned];
			if (NULL == apEnumAccess)
			{
				hr = E_OUTOFMEMORY;
				goto CLEANUP;
			}
			SecureZeroMemory(apEnumAccess,
				dwNumReturned * sizeof(IWbemObjectAccess*));
			dwNumObjects = dwNumReturned;

			if (FAILED(hr = pEnum->GetObjects(0L,
				dwNumObjects,
				apEnumAccess,
				&dwNumReturned)))
			{
				goto CLEANUP;
			}
		}
		else
		{
			if (hr == WBEM_S_NO_ERROR)
			{
				hr = WBEM_E_NOT_FOUND;
				goto CLEANUP;
			}
		}

		// First time through, get the handles.
		if (0 == x)
		{
			CIMTYPE VirtualBytesType;
			CIMTYPE ProcessHandleType;
			if (FAILED(hr = apEnumAccess[0]->GetPropertyHandle(
				L"VirtualBytes",
				&VirtualBytesType,
				&lVirtualBytesHandle)))
			{
				goto CLEANUP;
			}
			if (FAILED(hr = apEnumAccess[0]->GetPropertyHandle(
				L"IDProcess",
				&ProcessHandleType,
				&lIDProcessHandle)))
			{
				goto CLEANUP;
			}
		}

		for (i = 0; i < dwNumReturned; i++)
		{
			if (FAILED(hr = apEnumAccess[i]->ReadDWORD(
				lVirtualBytesHandle,
				&dwVirtualBytes)))
			{
				goto CLEANUP;
			}
			if (FAILED(hr = apEnumAccess[i]->ReadDWORD(
				lIDProcessHandle,
				&dwIDProcess)))
			{
				goto CLEANUP;
			}

			wprintf(L"Process ID %lu is using %lu bytes\n",
				dwIDProcess, dwVirtualBytes);

			// Done with the object
			apEnumAccess[i]->Release();
			apEnumAccess[i] = NULL;
		}

		if (NULL != apEnumAccess)
		{
			delete[] apEnumAccess;
			apEnumAccess = NULL;
		}

		// Sleep for a second.
		Sleep(1000);
	}
	// exit loop here
CLEANUP:

	if (NULL != bstrNameSpace)
	{
		SysFreeString(bstrNameSpace);
	}

	if (NULL != apEnumAccess)
	{
		for (i = 0; i < dwNumReturned; i++)
		{
			if (apEnumAccess[i] != NULL)
			{
				apEnumAccess[i]->Release();
				apEnumAccess[i] = NULL;
			}
		}
		delete[] apEnumAccess;
	}
	if (NULL != pWbemLocator)
	{
		pWbemLocator->Release();
	}
	if (NULL != pNameSpace)
	{
		pNameSpace->Release();
	}
	if (NULL != pEnum)
	{
		pEnum->Release();
	}
	if (NULL != pConfig)
	{
		pConfig->Release();
	}
	if (NULL != pRefresher)
	{
		pRefresher->Release();
	}

	CoUninitialize();

	if (FAILED(hr))
	{
		wprintf(L"Error status=%08x\n", hr);
	}

	return 1;
}