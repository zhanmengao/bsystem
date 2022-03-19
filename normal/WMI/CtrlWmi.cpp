#include <atlbase.h>
#include "ctrlwmi.h"
#include<ATLComTime.h>
#include"WMIDef.h"
#include"../../../../C++space/StringTools/StringTools/WinStrUtils.h"
#include"WMI.h"

CCtrlWmi::CCtrlWmi()
{
	m_pIWbemServices = NULL;
	m_pEnumClassObject = NULL;
	m_pClassObject = NULL;
	pIWbemLocator = NULL;
}



CCtrlWmi::~CCtrlWmi()
{
	DisconnectWMI();
	CoUninitialize();
}

BOOL CCtrlWmi::Init()
{
	m_hResult = CoInitializeEx(0, COINIT_MULTITHREADED); // Initialize COM.
	//m_hResult = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(m_hResult))
	{
		return FALSE;
	}
	m_hResult = CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,							//change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
		NULL);
	if (SUCCEEDED(m_hResult) || RPC_E_TOO_LATE == m_hResult)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CCtrlWmi::ConnectWMI(LPCTSTR lpstrNameSpace)
{
	try
	{
		DisconnectWMI();
		BOOL bResult = TRUE;
		do
		{
			CComBSTR pNamespace = CComBSTR(lpstrNameSpace);
			if ((m_hResult = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
				IID_IWbemLocator, (LPVOID *)&pIWbemLocator)) == S_OK)
			{
				if ((m_hResult = pIWbemLocator->ConnectServer(BSTR(pNamespace),
					NULL,  // using current account for simplicity
					NULL,	// using current password for simplicity
					0L,	// locale
					0L,	// securityFlags
					NULL,	// authority (domain for NTLM)
					NULL,	// context
					&m_pIWbemServices)) == S_OK)
				{
					bResult = TRUE;
				}
				else
				{
					bResult = FALSE;
					break;
				}

				m_hResult = CoSetProxyBlanket(m_pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
					RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

				if (FAILED(m_hResult))
				{
					bResult = FALSE;
					break;
				}
			}
			else
			{
				bResult = FALSE;
				break;
			}
		} while (0);
		if (!bResult)
		{
			DisconnectWMI();
		}
		return bResult;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		DisconnectWMI();
		return FALSE;
	}
}



BOOL CCtrlWmi::DisconnectWMI()
{
	try
	{
		if (m_pClassObject)
		{
			m_pClassObject->Release();
			m_pClassObject = NULL;
		}
		if (m_pIWbemServices)
		{
			m_pIWbemServices->Release();
			m_pIWbemServices = NULL;
		}
		if (m_pEnumClassObject)
		{
			m_pEnumClassObject->Release();
			m_pEnumClassObject = NULL;
		}
		if (pIWbemLocator)
		{
			pIWbemLocator->Release();
			pIWbemLocator = NULL;
		}
		return TRUE;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return FALSE;
	}
}



BOOL CCtrlWmi::BeginEnumClassObject(LPCTSTR lpstrObject)
{
	try
	{
		// Get the object class
		CComBSTR className = CComBSTR(lpstrObject);
		if ((!m_pIWbemServices))
			return FALSE;
		if (m_pEnumClassObject)
		{
			m_pEnumClassObject->Release();
			m_pEnumClassObject = NULL;
		}
		m_hResult = m_pIWbemServices->CreateInstanceEnum(BSTR(className),			// name of class
			0, NULL, &m_pEnumClassObject);// pointer to enumerator
		if (FAILED(m_hResult))
			return FALSE;
		return TRUE;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		CloseEnumClassObject();
		return FALSE;
	}
}



BOOL CCtrlWmi::MoveNextEnumClassObject()
{
	try
	{
		ULONG uReturned = 1;
		if (m_pEnumClassObject == NULL)
			return FALSE;
		if (m_pClassObject!=NULL)
		{
			m_pClassObject->Release();
			m_pClassObject = NULL;
		}
		m_hResult = m_pEnumClassObject->Next(10000,			// timeout in 10 seconds
			1,				// return just one storage device
			&m_pClassObject,// pointer to storage device
			&uReturned);	// number obtained: one or zero
		if (SUCCEEDED(m_hResult) && (uReturned == 1))
			return TRUE;
		return FALSE;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return FALSE;
	}
}



BOOL CCtrlWmi::CloseEnumClassObject()
{
	try
	{
		if (m_pClassObject != NULL)
		{
			m_pClassObject->Release();
			m_pClassObject = NULL;
		}
		if (m_pEnumClassObject!=NULL)
		{
			m_pEnumClassObject->Release();
			m_pEnumClassObject = NULL;
		}
		return TRUE;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return FALSE;
	}
}
std::string CCtrlWmi::GetString(LPCTSTR lpstrProperty)
{
	string ret;
	VARIANT pVal;
	try
	{
		CComBSTR propName = CComBSTR(lpstrProperty);

		VariantInit(&pVal);
		CIMTYPE pType;
		if (!m_pClassObject)
		{
			VariantClear(&pVal);
			return NULL;
		}
		m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (SUCCEEDED(m_hResult) && pVal.bstrVal != NULL)
		{
			char* pData = _com_util::ConvertBSTRToString(pVal.bstrVal);
			ret = pData;
			if (pData)
			{
				delete[] pData;
				pData = NULL;
			}
			VariantClear(&pVal);
		}
		return ret;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		VariantClear(&pVal);
		return ret;
	}
	return ret;
}
BOOL CCtrlWmi::GetBool(LPCTSTR lpstrProperty)
{
	BOOL ret = 0;
	VARIANT pVal;
	try
	{
		CComBSTR propName = CComBSTR(lpstrProperty);

		VariantInit(&pVal);
		CIMTYPE pType;
		if (!m_pClassObject)
		{
			VariantClear(&pVal);
			return NULL;
		}
		m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (SUCCEEDED(m_hResult))
		{
			ret = V_BOOL(&pVal);
			VariantClear(&pVal);
		}
		return ret;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		VariantClear(&pVal);
		return ret;
	}
	return ret;
}
stdstr CCtrlWmi::GetClassObjectStringValue(LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	do
	{
		try
		{
			CComBSTR propName = CComBSTR(lpstrProperty);
			VariantInit(&pVal);
			CIMTYPE pType;
			if (!m_pClassObject)
			{
				m_csResult = _T("");
			}
			m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
			if (SUCCEEDED(m_hResult))
			{
				LPCTSTR p = strCimValue(pVal, pType);
				if (p)
				{
					m_csResult = p;
				}
				else
				{
					m_csResult = _T("");
				}
				break;
			}
			m_csResult = _T("");
			break;
		}
		catch (...)
		{
			m_hResult = WBEM_E_FAILED;
			m_csResult = _T("");
		}
	} while (false);
	VariantClear(&pVal);
	return m_csResult;
}



DWORD CCtrlWmi::GetClassObjectDwordValue(LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR propName = CComBSTR(lpstrProperty);
		VariantInit(&pVal);
		CIMTYPE pType;
		if (!m_pClassObject)
			return 0;
		m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (SUCCEEDED(m_hResult))
			VariantClear(&pVal);
		return dwCimValue(pVal, pType);
		return 0;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		VariantClear(&pVal);
		return 0;
	}
}

__int64 CCtrlWmi::GetClassObjectI64Value(LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR propName = CComBSTR(lpstrProperty);
		VariantInit(&pVal);
		CIMTYPE pType;
		if (!m_pClassObject)
			VariantClear(&pVal);
		return 0;
		m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (SUCCEEDED(m_hResult))
			VariantClear(&pVal);
		return i64CimValue(pVal, pType);
		VariantClear(&pVal);
		return 0;
	}
	catch (...)
	{
		VariantClear(&pVal);
		m_hResult = WBEM_E_FAILED;
		return 0;
	}
}



unsigned __int64 CCtrlWmi::GetClassObjectU64Value(LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR propName = CComBSTR(lpstrProperty);
		VariantInit(&pVal);
		CIMTYPE pType;
		if (!m_pClassObject)
			return 0;
		m_hResult = m_pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (SUCCEEDED(m_hResult))
		{
			unsigned __int64 dwVal = u64CimValue(pVal, pType);
			VariantClear(&pVal);
			return dwVal;
		}
		VariantClear(&pVal);
		return 0;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		VariantClear(&pVal);
		return 0;
	}
}



stdstr CCtrlWmi::GetRefElementClassObjectStringValue(LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR elementName = CComBSTR(lpstrRefElement);
		CComBSTR propName = CComBSTR(lpstrProperty);
		VariantInit(&pVal);
		CIMTYPE pType;
		IWbemClassObject *pClassObject;
		if (!m_pClassObject)
		{
			VariantClear(&pVal);
			return NULL;
		}
		m_hResult = m_pClassObject->Get(BSTR(elementName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
			VariantClear(&pVal);
		return NULL;
		stdstr str = strCimValue(pVal, pType);
		if (str.empty())
			VariantClear(&pVal);
		return NULL;
		m_hResult = m_pIWbemServices->GetObject(CComBSTR(str.c_str()),
			WBEM_FLAG_RETURN_WBEM_COMPLETE, NULL, &pClassObject, NULL);
		if (FAILED(m_hResult))
		{
			VariantClear(&pVal);
			pClassObject->Release();
			return NULL;
		}
		m_hResult = pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
		{
			pClassObject->Release();
			VariantClear(&pVal);
			return NULL;
		}
		if (pType == 101)
		{
			m_csResult = strCimValue(pVal, pType);
		}
		else
		{
			m_csResult = strCimArrayValue(pVal, pType);
		}
		VariantClear(&pVal);
		pClassObject->Release();
		return m_csResult;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return NULL;
	}
}



DWORD CCtrlWmi::GetRefElementClassObjectDwordValue(LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR elementName = CComBSTR(lpstrRefElement);
		CComBSTR propName = CComBSTR(lpstrProperty);
		CString	csObject;
		VariantInit(&pVal);
		CIMTYPE pType;
		IWbemClassObject *pClassObject;
		static DWORD dwResult;
		if (!m_pClassObject)
		{
			VariantClear(&pVal);
			return 0;
		}
		m_hResult = m_pClassObject->Get(BSTR(elementName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
			VariantClear(&pVal);
		return 0;
		csObject = strCimValue(pVal, pType);
		if (csObject.IsEmpty())
			VariantClear(&pVal);
		return 0;
		m_hResult = m_pIWbemServices->GetObject(CComBSTR(csObject),
			WBEM_FLAG_RETURN_WBEM_COMPLETE,
			NULL,
			&pClassObject,
			NULL);
		if (FAILED(m_hResult))
		{
			VariantClear(&pVal);
			pClassObject->Release();
			return 0;
		}
		m_hResult = pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
			dwResult = 0;
		else
			dwResult = dwCimValue(pVal, pType);
		pClassObject->Release();
		VariantClear(&pVal);
		return dwResult;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return 0;
	}
}



__int64 CCtrlWmi::GetRefElementClassObjectI64Value(LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR elementName = CComBSTR(lpstrRefElement);
		CComBSTR propName = CComBSTR(lpstrProperty);
		CString	 csObject;
		VariantInit(&pVal);
		CIMTYPE pType;
		IWbemClassObject *pClassObject;
		static __int64 i64Result;
		if (!m_pClassObject)
		{
			VariantInit(&pVal);
			return 0;
		}
		VariantClear(&pVal);
		m_hResult = m_pClassObject->Get(BSTR(elementName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
			VariantInit(&pVal);
		return 0;
		csObject = strCimValue(pVal, pType);
		if (csObject.IsEmpty())
			VariantInit(&pVal);
		return 0;
		m_hResult = m_pIWbemServices->GetObject(CComBSTR(csObject),

			WBEM_FLAG_RETURN_WBEM_COMPLETE,

			NULL,

			&pClassObject,

			NULL);

		if (FAILED(m_hResult))
		{
			pClassObject->Release();
			VariantInit(&pVal);
			return 0;
		}
		m_hResult = pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
			i64Result = 0;
		else
			i64Result = i64CimValue(pVal, pType);
		VariantClear(&pVal);
		pClassObject->Release();
		return i64Result;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		return 0;
	}
}



unsigned __int64 CCtrlWmi::GetRefElementClassObjectU64Value(LPCTSTR lpstrRefElement, LPCTSTR lpstrProperty)
{
	VARIANT pVal;
	try
	{
		CComBSTR elementName = CComBSTR(lpstrRefElement);
		CComBSTR propName = CComBSTR(lpstrProperty);
		CString	csObject;
		VariantInit(&pVal);
		CIMTYPE pType;
		IWbemClassObject *pClassObject;
		static unsigned __int64 u64Result;
		if (!m_pClassObject)
		{
			VariantClear(&pVal);
			return 0;
		}
		m_hResult = m_pClassObject->Get(BSTR(elementName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
		{
			VariantClear(&pVal);
			return 0;
		}
		csObject = strCimValue(pVal, pType);
		if (csObject.IsEmpty())
			VariantClear(&pVal);
		return 0;
		m_hResult = m_pIWbemServices->GetObject(CComBSTR(csObject),
			WBEM_FLAG_RETURN_WBEM_COMPLETE,
			NULL,
			&pClassObject,
			NULL);
		if (FAILED(m_hResult))
		{
			VariantClear(&pVal);
			pClassObject->Release();
			return 0;
		}
		m_hResult = pClassObject->Get(BSTR(propName), 0L, &pVal, &pType, NULL);
		if (FAILED(m_hResult))
		{
			u64Result = 0;
		}
		else
		{
			u64Result = u64CimValue(pVal, pType);
		}
		VariantClear(&pVal);
		pClassObject->Release();
		return u64Result;
	}
	catch (...)
	{
		m_hResult = WBEM_E_FAILED;
		VariantClear(&pVal);
		return 0;
	}
}



HRESULT CCtrlWmi::GetLastErrorWMI()
{
	return m_hResult;
}



LPCTSTR CCtrlWmi::strCimValue(VARIANT &pVal, CIMTYPE &pType)
{
	COleDateTime pOleDate;
	CString csResult;
	if ((pVal.vt == VT_NULL) || (pVal.vt == VT_EMPTY))
		csResult.Empty();
	else
		switch (pType)
		{
		case CIM_ILLEGAL:
			csResult = _T("CIM_ILLEGAL");
			break;
		case CIM_EMPTY:
			csResult = _T("");
			break;
		case CIM_SINT8:
			csResult.Format(_T("%hd"), V_I1(&pVal));
			break;
		case CIM_UINT8:
			csResult.Format(_T("%hu"), V_UI1(&pVal));
			break;
		case CIM_SINT16:
			csResult.Format(_T("%d"), V_I2(&pVal));
			break;
		case CIM_UINT16:
			csResult.Format(_T("%u"), V_UI2(&pVal));
			break;
		case CIM_SINT32:
			csResult.Format(_T("%ld"), V_I4(&pVal));
			break;
		case CIM_UINT32:
			csResult.Format(_T("%lu"), V_UI4(&pVal));
			break;
		case CIM_SINT64:
			csResult = V_BSTR(&pVal);
			break;
		case CIM_UINT64:
			csResult = V_BSTR(&pVal);
			break;
		case CIM_REAL32:
			csResult.Format(_T("%e"), V_R4(&pVal));
			break;
		case CIM_REAL64:
			csResult.Format(_T("%le"), V_R8(&pVal));
			break;
		case CIM_BOOLEAN:
			csResult = (V_BOOL(&pVal) ? _T("TRUE") : _T("FALSE"));
			break;
		case CIM_STRING:
			csResult = V_BSTR(&pVal);
			break;
		case CIM_DATETIME:
			pOleDate = COleDateTime(pVal);
			if (pOleDate.GetStatus() == COleDateTime::valid)
				csResult = pOleDate.Format(VAR_DATEVALUEONLY);
			else
				csResult = V_BSTR(&pVal);
			break;
		case CIM_REFERENCE:
			csResult = V_BSTR(&pVal);
			break;
		case CIM_CHAR16:
			csResult = V_BSTR(&pVal);
			break;
		case CIM_OBJECT:
			csResult = _T("CIM_OBJECT");
			break;
		default:
			csResult = strVariantArray(pVal);
			break;
		}
	return csResult;
}



DWORD CCtrlWmi::dwCimValue(VARIANT &pVal, CIMTYPE &pType)
{
	if ((pVal.vt == VT_NULL) || (pVal.vt == VT_EMPTY))
		return 0;
	else
		switch (pType)
		{
		case CIM_SINT8:
			return V_I1(&pVal);
		case CIM_UINT8:
			return V_UI1(&pVal);
		case CIM_SINT16:
			return V_I2(&pVal);
		case CIM_UINT16:
			return V_UI2(&pVal);
		case CIM_SINT32:
			return V_I4(&pVal);
		case CIM_UINT32:
			return V_UI4(&pVal);
		case CIM_BOOLEAN:
			return (V_BOOL(&pVal) ? 1 : 0);
		default:
			return 0;
		}
}



__int64 CCtrlWmi::i64CimValue(VARIANT &pVal, CIMTYPE &pType)
{
	CString csResult;
	if ((pVal.vt == VT_NULL) || (pVal.vt == VT_EMPTY))
		return 0;
	else
		switch (pType)
		{
		case CIM_SINT8:

			return V_I1(&pVal);

		case CIM_UINT8:

			return V_UI1(&pVal);

		case CIM_SINT16:

			return V_I2(&pVal);

		case CIM_UINT16:

			return V_UI2(&pVal);

		case CIM_SINT32:

			return V_I4(&pVal);

		case CIM_UINT32:

			return V_UI4(&pVal);

		case CIM_SINT64:

			csResult = V_BSTR(&pVal);

			return _ttoi64(csResult);

		case CIM_UINT64:

			csResult = V_BSTR(&pVal);

			return _ttoi64(csResult);

		case CIM_BOOLEAN:

			return (V_BOOL(&pVal) ? 1 : 0);

		default:

			return 0;

		}

}



unsigned __int64 CCtrlWmi::u64CimValue(VARIANT &pVal, CIMTYPE &pType)
{
	CString csResult;
	if ((pVal.vt == VT_NULL) || (pVal.vt == VT_EMPTY))

		return 0;

	else

		switch (pType)

		{

		case CIM_SINT8:

			return V_I1(&pVal);

		case CIM_UINT8:

			return V_UI1(&pVal);

		case CIM_SINT16:

			return V_I2(&pVal);

		case CIM_UINT16:

			return V_UI2(&pVal);

		case CIM_SINT32:

			return V_I4(&pVal);

		case CIM_UINT32:

			return V_UI4(&pVal);

		case CIM_SINT64:

			csResult = V_BSTR(&pVal);

			return _ttoi64(csResult);

		case CIM_UINT64:

			csResult = V_BSTR(&pVal);

			return _ttoi64(csResult);

		case CIM_BOOLEAN:

			return (V_BOOL(&pVal) ? 1 : 0);

		default:

			return 0;

		}

}



LPCTSTR CCtrlWmi::strCimArrayValue(VARIANT &pVal, CIMTYPE &pType)
{
	LONG dwSLBound = 0;
	LONG dwSUBound = 0;
	VARIANT v;
	VariantInit(&v);
	LONG i;
	CString strRet;
	HRESULT hr;
	CIMTYPE cimTypeWithoutArray;
	if (!V_ISARRAY(&pVal))
		return strCimValue(pVal, pType);
	if ((V_ARRAY(&pVal))->cDims != 1)
		return NULL;
	if ((V_ARRAY(&pVal))->rgsabound[0].cElements == 0)
		return NULL;
	hr = SafeArrayGetLBound(V_ARRAY(&pVal), 1, (long FAR *)&dwSLBound);
	if (FAILED(hr))
		return NULL;
	hr = SafeArrayGetUBound(V_ARRAY(&pVal), 1, (long FAR *)&dwSUBound);
	if (FAILED(hr))
		return NULL;
	cimTypeWithoutArray = pType ^ CIM_FLAG_ARRAY;
	strRet = _T("");
	v.vt = (unsigned short)cimTypeWithoutArray;

	DECIMAL_SETZERO(v.decVal);
	for (i = dwSLBound; i <= dwSUBound; i++)
	{
		hr = SafeArrayGetElement(V_ARRAY(&pVal), (long FAR *)&i, &v.lVal);
		if (FAILED(hr))
			continue;
		strRet += strCimValue(v, cimTypeWithoutArray);
		if (i < dwSUBound)
		{
			strRet += _T(";");
		}
		DECIMAL_SETZERO(v.decVal);
	}
	return(strRet);
}



LPCTSTR CCtrlWmi::strVariant(VARIANT pVal)
{
	static CString strRet;
	strRet = _T("N/A");

	switch (pVal.vt)
	{

	case VT_EMPTY:

	case VT_NULL:

		strRet = _T("");

		break;

	case VT_I1:

		strRet.Format(_T("%hd"), V_I2(&pVal));

		break;

	case VT_I2:

		strRet.Format(_T("%d"), V_I2(&pVal));

		break;

	case VT_I4:

		strRet.Format(_T("%ld"), V_I4(&pVal));

		break;

	case VT_I8:

		strRet.Format(_T("%I64d"), V_I8(&pVal));

		break;

	case VT_UI1:

		strRet.Format(_T("%hu"), V_UI1(&pVal));

		break;

	case VT_UI2:

		strRet.Format(_T("%u"), V_UI2(&pVal));

		break;

	case VT_UI4:

		strRet.Format(_T("%lu"), V_UI4(&pVal));

		break;

	case VT_UI8:

		strRet.Format(_T("%I64u"), V_UI8(&pVal));

		break;

	case VT_INT:

		strRet.Format(_T("%d"), V_INT(&pVal));

		break;

	case VT_UINT:

		strRet.Format(_T("%u"), V_UINT(&pVal));

		break;

	case VT_R4:

		strRet.Format(_T("%e"), V_R4(&pVal));

		break;

	case VT_R8:

		strRet.Format(_T("%le"), V_R8(&pVal));

		break;

	case VT_CY:
		//strRet = COleCurrency(pVal).Format();
		break;
	case VT_DATE:
		strRet = COleDateTime(pVal).Format(VAR_DATEVALUEONLY);
		break;
	case VT_BSTR:
		//strRet = V_BSTRT(&pVal);
		break;
	case VT_DISPATCH:
		strRet = _T("VT_DISPATCH");
		break;
	case VT_ERROR:
		strRet = _T("VT_ERROR");
		break;
	case VT_BOOL:
		return (V_BOOL(&pVal) ? _T("TRUE") : _T("FALSE"));
	case VT_VARIANT:
		strRet = _T("VT_VARIANT");
		break;
	case VT_UNKNOWN:
		strRet = _T("VT_UNKNOWN");
		break;
	case VT_VOID:
		strRet = _T("VT_VOID");
		break;
	case VT_HRESULT:
		strRet = _T("VT_HRESULT");
		break;
	case VT_PTR:
		strRet = _T("VT_PTR");
		break;
	case VT_SAFEARRAY:
		strRet = _T("VT_SAFEARRAY");
		break;
	case VT_CARRAY:
		strRet = _T("VT_CARRAY");
		break;
	case VT_USERDEFINED:
		strRet = _T("VT_USERDEFINED");
		break;
	case VT_LPSTR:
		strRet = _T("VT_LPSTR");
		break;
	case VT_LPWSTR:
		strRet = _T("VT_LPWSTR");
		break;
	case VT_FILETIME:
		strRet = _T("VT_FILETIME");
		break;
	case VT_BLOB:
		strRet = _T("VT_BLOB");
		break;
	case VT_STREAM:
		strRet = _T("VT_STREAM");
		break;
	case VT_STORAGE:
		strRet = _T("VT_STORAGE");
		break;
	case VT_STREAMED_OBJECT:
		strRet = _T("VT_STREAMED_OBJECT");
		break;
	case VT_STORED_OBJECT:
		strRet = _T("VT_STORED_OBJECT");
		break;
	case VT_BLOB_OBJECT:
		strRet = _T("VT_BLOB_OBJECT");
		break;
	case VT_CF:
		strRet = _T("VT_CF");
		break;
	case VT_CLSID:
		strRet = _T("VT_CLSID");
		break;
	}
	return strRet;

}



LPCTSTR CCtrlWmi::strVariantArray(VARIANT var)

{

	LONG dwSLBound = 0;

	LONG dwSUBound = 0;

	VARIANT v;
	VariantInit(&v);

	LONG i;

	static CString strRet;

	HRESULT hr;



	if (!V_ISARRAY(&var))

		return strVariant(var);



	//

	// Check that there is only one dimension in this array

	//



	if ((V_ARRAY(&var))->cDims != 1)

		return NULL;



	//

	// Check that there is atleast one element in this array

	//



	if ((V_ARRAY(&var))->rgsabound[0].cElements == 0)

		return NULL;



	//

	// We know that this is a valid single dimension array

	//



	hr = SafeArrayGetLBound(V_ARRAY(&var), 1, (long FAR *)&dwSLBound);

	if (FAILED(hr))

		return NULL;



	hr = SafeArrayGetUBound(V_ARRAY(&var), 1, (long FAR *)&dwSUBound);

	if (FAILED(hr))

		return NULL;



	strRet = _T("");

	// VariantInit(&v);

	DECIMAL_SETZERO(v.decVal);
	for (i = dwSLBound; i <= dwSUBound; i++)
	{
		hr = SafeArrayGetElement(V_ARRAY(&var), (long FAR *)&i, &v);
		if (FAILED(hr))
			continue;
		if (i < dwSUBound)
		{
			strRet += strVariant(v);
			strRet += _T(";");
		}
		else
		{
			strRet += strVariant(v);
		}
		// VariantClear(&v);
		DECIMAL_SETZERO(v.decVal);

	}

	return(strRet);

}

