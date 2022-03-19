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
	HRESULT InitWmi();    //��ʼ��WMI
	HRESULT ReleaseWmi(); //�ͷ�


	BOOL GetSingleItemInfo(CString, CString, CString&);
	BOOL GetGroupItemInfo(CString, CString[], int, CString&);

private:
	void VariantToString(const LPVARIANT, CString &) const;//��Variant���͵ı���ת��ΪCString
private:
	IEnumWbemClassObject* m_pEnumClsObj;
	IWbemClassObject* m_pWbemClsObj;
	IWbemServices* m_pWbemSvc;
	IWbemLocator* m_pWbemLoc;
};
