#pragma once
#include<string>
#include <atlbase.h>
#include <WbemIdl.h>
#include<assert.h>
#define CHECKHR(hr) assert(hr)
using namespace std;

class CWMIBase
{
public:
	CWMIBase()
	{

	}
	~CWMIBase(void);
public:
	HRESULT ExcuteFun();
protected:
	VOID SetNamespace(wstring wstrNamespace);
private:
	HRESULT InitialCom();
	HRESULT SetComSecLevels();
	HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);
	HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc, CComPtr<IWbemServices>& pSvc);
	HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);
	virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;
private:
	wstring m_wstrNamespace;
};
