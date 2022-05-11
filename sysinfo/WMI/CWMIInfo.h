#pragma once
#include <afxpriv.h>
#include <WbemIdl.h>
#pragma comment(lib,"WbemUuid.lib")

class CWmiInfo
{
public:
	CWmiInfo(void);
	~CWmiInfo(void);

public:
	HRESULT InitWmi();    //初始化WMI
	HRESULT ReleaseWmi(); //释放


	BOOL GetSingleItemInfo(CString, CString, CString&);
	BOOL GetGroupItemInfo(CString, CString[], int, CString&);

private:
	void VariantToString(const LPVARIANT, CString &) const;//将Variant类型的变量转换为CString
private:
	IEnumWbemClassObject* m_pEnumClsObj;
	IWbemClassObject* m_pWbemClsObj;
	IWbemServices* m_pWbemSvc;
	IWbemLocator* m_pWbemLoc;
};
