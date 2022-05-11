#pragma once
#include <wbemcli.h>
#include<string>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"


class CCtrlWmi
{
public:
	CCtrlWmi(void);
	~CCtrlWmi(void);
	BOOL Init();
	BOOL ConnectWMI( LPCTSTR lpstrNameSpace);
	BOOL DisconnectWMI();

	BOOL BeginEnumClassObject( LPCTSTR lpstrObject);
	BOOL MoveNextEnumClassObject();
	BOOL CloseEnumClassObject();

	std::string GetString(LPCTSTR lpstrProperty);
	BOOL GetBool(LPCTSTR lpstrProperty);
	stdstr GetClassObjectStringValue(LPCTSTR lpstrProperty);
	stdstr GetRefElementClassObjectStringValue( LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty);

	DWORD GetClassObjectDwordValue( LPCTSTR lpstrProperty);
	DWORD GetRefElementClassObjectDwordValue( LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty);

	__int64 GetClassObjectI64Value( LPCTSTR lpstrProperty);
	__int64 GetRefElementClassObjectI64Value( LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty);

	unsigned __int64 GetClassObjectU64Value( LPCTSTR lpstrProperty);
	unsigned __int64 GetRefElementClassObjectU64Value( LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty);

	HRESULT GetLastErrorWMI();

protected: // Methods
	LPCTSTR strVariant( VARIANT pVal);
	LPCTSTR strVariantArray( VARIANT pVal);
	LPCTSTR strCimArrayValue( VARIANT &pVal, CIMTYPE &pType);
	LPCTSTR strCimValue( VARIANT &pVal, CIMTYPE &pType);
	DWORD dwCimValue( VARIANT &pVal, CIMTYPE &pType);
	__int64 i64CimValue( VARIANT &pVal, CIMTYPE &pType);
	unsigned __int64 u64CimValue( VARIANT &pVal, CIMTYPE &pType);

protected: // Attributes
	IWbemServices *m_pIWbemServices = NULL;
	IEnumWbemClassObject *m_pEnumClassObject = NULL;
	IWbemClassObject *m_pClassObject = NULL;
	IWbemLocator *pIWbemLocator = NULL;
	HRESULT m_hResult;
	stdstr m_csResult;
};
