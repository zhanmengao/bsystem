#include "stdafx.h"
#include <vector>
#include "WarInfo.h"
#pragma once

using namespace std;

class COtherWay
{
public:
	COtherWay(void);
	~COtherWay(void);

	// OS information
	UINT GetOS( CString &csName, CString &csVersion);

	BOOL GetNetworkAdaptersByIPHelper( vector<NetworkAdaptertdll> &vecMyList);
	BOOL GetNetworkAdaptersBySNMP( vector<NetworkAdaptertdll> &vecMyList);
};
