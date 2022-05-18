#pragma once
#pragma once

#include <afxwin.h>
#include <iostream>
#include <map>
#include <vector>
#include"../../../C++space/StringTools/StringTools/StringHead.h"

class CIni
{
public:
	// Ini�ļ��еļ�ֵ�Ա�
	typedef std::map<stdstr, stdstr> KValueTable;
	// Ini�ļ��еĽ�
	typedef struct Section
	{
		stdstr sectionName;
		KValueTable kValueTable;
	}*PSection;
	// Ini�ļ��еĽڱ�
	typedef std::vector<Section> SectionTable;

	CIni();
	~CIni();

	// ����Ini�ļ�������ֵ  -2:�ļ���ȡ����; -1:��֧�ֵı���; 0:��ȷ; > 0:�������������(�ڼ���)
	int Load(TCHAR *fileName);
	// ����Ini�ļ�
	bool Save(TCHAR *fileName);

	// ��սڱ�
	void Clear() { m_sectionTable.clear(); }

	// ��ӽ�
	bool AddSection(TCHAR *sectionName);

	// ��Ӽ�ֵ
	bool AddKValue(TCHAR *sectionName, TCHAR *key, TCHAR *value);

	// ͨ�������ͼ�����ȡֵ
	const TCHAR *GetValue(TCHAR *sectionName, TCHAR *key);

	// ͨ�������ͼ����޸�ֵ
	bool UpdateValue(TCHAR *sectionName, TCHAR *key, TCHAR *value);

	// ��ȡ�ڱ�
	const SectionTable &GetSectionTable() const { return m_sectionTable; }

	// ������������ 0�����ܴ���ı��뷵��-1�� ���򷵻�ʧ�ܵ�������Ŀǰֻ�ܽ���С��unicode���룩
	int Parse(TCHAR *text, int size, SectionTable &sectionTable);

	// ini�ļ����ݣ�����ʽ��
	stdstr ToString();

private:
	SectionTable m_sectionTable;        // �ڱ�
};