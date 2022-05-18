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
	// Ini文件中的键值对表
	typedef std::map<stdstr, stdstr> KValueTable;
	// Ini文件中的节
	typedef struct Section
	{
		stdstr sectionName;
		KValueTable kValueTable;
	}*PSection;
	// Ini文件中的节表
	typedef std::vector<Section> SectionTable;

	CIni();
	~CIni();

	// 加载Ini文件，返回值  -2:文件读取错误; -1:不支持的编码; 0:正确; > 0:解析错误的行数(第几号)
	int Load(TCHAR *fileName);
	// 保存Ini文件
	bool Save(TCHAR *fileName);

	// 清空节表
	void Clear() { m_sectionTable.clear(); }

	// 添加节
	bool AddSection(TCHAR *sectionName);

	// 添加键值
	bool AddKValue(TCHAR *sectionName, TCHAR *key, TCHAR *value);

	// 通过节名和键名获取值
	const TCHAR *GetValue(TCHAR *sectionName, TCHAR *key);

	// 通过节名和键名修改值
	bool UpdateValue(TCHAR *sectionName, TCHAR *key, TCHAR *value);

	// 获取节表
	const SectionTable &GetSectionTable() const { return m_sectionTable; }

	// 解析正常返回 0，不能处理的编码返回-1， 否则返回失败的行数（目前只能解析小端unicode编码）
	int Parse(TCHAR *text, int size, SectionTable &sectionTable);

	// ini文件内容（树形式）
	stdstr ToString();

private:
	SectionTable m_sectionTable;        // 节表
};