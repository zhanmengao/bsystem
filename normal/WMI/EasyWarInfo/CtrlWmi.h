#pragma once
#include "wbemcli.h"

class CCtrlWmi
{
public:
	CCtrlWmi(void);
	~CCtrlWmi(void);
	BOOL ConnectWMI( LPCTSTR lpstrNameSpace);
	BOOL DisconnectWMI();

	BOOL BeginEnumClassObject( LPCTSTR lpstrObject);
	BOOL MoveNextEnumClassObject();
	BOOL CloseEnumClassObject();

	LPCTSTR GetClassObjectStringValue( LPCTSTR lpstrProperty);
	LPCTSTR GetRefElementClassObjectStringValue( LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty);

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
	IWbemServices *m_pIWbemServices;
	IEnumWbemClassObject *m_pEnumClassObject;
	IWbemClassObject *m_pClassObject;
	HRESULT m_hResult;
	CString m_csResult;
};
