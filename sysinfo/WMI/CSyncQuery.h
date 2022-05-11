#pragma once
#include"WmiBase.h"

class CSynQuery : public CWMIBase
{
public:
	CSynQuery(const wstring& wszNamespace, const wstring& wszWQLQuery)
	{

	}
	~CSynQuery(void)
	{

	}
private:
	HRESULT Excute(CComPtr<IWbemServices> pSvc);
	HRESULT DealWithIWbemClassObject(CComPtr<IWbemClassObject> pclsObj);
	virtual HRESULT DealWithSingleItem(CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor) = 0;
private:
	wstring m_wszWQLQuery;
};
