// RepairPe.h: interface for the CRepairPe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPAIRPE_H__932C52A7_1A57_4F02_80D1_93A531D8610B__INCLUDED_)
#define AFX_REPAIRPE_H__932C52A7_1A57_4F02_80D1_93A531D8610B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRepairPe  
{
public:
	CString m_VirDump;
	CString m_StrVir;
	VOID Repair(char *pSrc, char *pDest);
	DWORD Align(DWORD dwSize, DWORD dwAlign);
	VOID BuildIat(char *pSrc, char *pDest);
	DWORD Rva2Fa(PIMAGE_NT_HEADERS pImgNtHdr, LPVOID lpBase, DWORD dwRva);
	BOOL DumpVir(char *strVir);
	VOID OneCc(DEBUG_EVENT *pDe, BYTE *bCode);
	VOID TwoCc(DEBUG_EVENT *pDe, BYTE *bCode);
	VOID ThreeCc(DEBUG_EVENT *pDe, BYTE *bCode);
	VOID Dump(DEBUG_EVENT *pDe, DWORD dwEntryPoint);
	CRepairPe(CString strVir);
	virtual ~CRepairPe();

};

#endif // !defined(AFX_REPAIRPE_H__932C52A7_1A57_4F02_80D1_93A531D8610B__INCLUDED_)
