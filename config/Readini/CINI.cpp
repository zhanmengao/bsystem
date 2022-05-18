#include "CINI.h"

#include <fstream>
#include"../../../C++space/StringTools/StringTools/StringHead.h"

CIni::CIni()
{
}

CIni::~CIni()
{
}

int CIni::Load(TCHAR * fileName)
{
	// 清空节表
	m_sectionTable.clear();

	// 打开文件
	CFile file;
	if (!file.Open(fileName, CFile::modeRead)) { return -1; }

	// 获取文件大小
	CFileStatus fileStatus;
	file.GetStatus(fileStatus);
	int fileSize = fileStatus.m_size;

	// 读取文件
	TCHAR *text = new TCHAR[fileSize / 2 + 10];
	int readLength = file.Read(text, fileSize);
	if (readLength != fileSize)
	{
		file.Close();

		return -1;
	}

	// 解析文件
	int errorRow = Parse(text, fileSize / 2, m_sectionTable);

	delete[] text;

	file.Close();

	return errorRow;
}

bool CIni::Save(TCHAR * fileName)
{
	CFile file;
	if (!file.Open(fileName, CFile::modeCreate | CFile::modeWrite)) { return false; }

	const TCHAR left = _T('[');                         // 节名左中括号
	const TCHAR right = _T(']');                        // 节名右中括号
	const TCHAR equal = _T('=');                        // 等号
	const TCHAR newLine[2] = { _T('\r'), _T('\n') };      // 回车换行

														  // 遍历节表
	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		file.Write(&left, 2);
		file.Write(it->sectionName.c_str(), it->sectionName.length() * 2);
		file.Write(&right, 2);
		file.Write(newLine, 4);

		// 遍历键值表
		for (auto i = it->kValueTable.begin(); i != it->kValueTable.end(); ++i)
		{
			file.Write(i->first.c_str(), i->first.length() * 2);
			file.Write(&equal, 2);
			file.Write(i->second.c_str(), i->second.length() * 2);
			file.Write(newLine, 4);
		}
	}

	return true;
}

bool CIni::AddSection(TCHAR *sectionName)
{
	stdstr wSectionName(sectionName);
	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		if (it->sectionName == wSectionName)
		{
			return false;
		}
	}

	Section section;
	section.sectionName = wSectionName;
	m_sectionTable.push_back(section);

	return true;
}

bool CIni::AddKValue(TCHAR *sectionName, TCHAR *key, TCHAR *value)
{
	stdstr wSectionName(sectionName);
	stdstr wKey(key);
	stdstr wValue(value);

	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		if (it->sectionName == wSectionName)
		{
			it->kValueTable[wKey] = wValue;

			return true;
		}

	}

	return false;
}

const TCHAR *CIni::GetValue(TCHAR * sectionName, TCHAR * key)
{
	stdstr wSectionName(sectionName);
	stdstr wKey(key);

	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		if (it->sectionName == wSectionName)
		{
			auto temp = it->kValueTable.find(wKey);
			if (temp == it->kValueTable.end())
			{
				return nullptr;
			}
			else
			{
				return temp->second.c_str();
			}
		}

	}

	return nullptr;
}

bool CIni::UpdateValue(TCHAR * sectionName, TCHAR * key, TCHAR * value)
{
	stdstr wSectionName(sectionName);
	stdstr wKey(key);
	stdstr wValue(value);

	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		if (it->sectionName == wSectionName)
		{
			auto temp = it->kValueTable.find(wKey);
			if (temp == it->kValueTable.end())
			{
				return false;
			}
			else
			{
				temp->second = wValue;

				return true;
			}
		}
	}

	return false;
}

int CIni::Parse(TCHAR * text, int size, SectionTable & sectionTable)
{
	TCHAR sectionName[100] = { 0 };       // 节名
	int sectionIndex = 0;               // 节名下标

	TCHAR key[100] = { 0 };             // 键
	int keyIndex = 0;                   // 键下标

	TCHAR value[2000] = { 0 };            // 值
	int valueIndex = 0;                 // 值下标

	bool isSection = false;             // 当前字符是节名
	bool isKey = false;                 // 当前字符是键
	bool isValue = false;               // 当前字符是值
	bool isComment = false;             // 当前字符是注释

										// 失败行数
	int errorRow = 0;

	// 小端
	if (*text == 0xFEFF)
	{
		++text;
		--size;
	}

	// 大端不能解析
	if (*text == 0xFFFE)
	{
		return -1;
	}

	for (int i = 0; i < size; ++i)
	{
		if (text[i] == _T('['))
		{
			if (!isComment)
			{
				if (isKey)
				{
					key[keyIndex] = text[i];
					++keyIndex;
				}
				else if (isValue)
				{
					value[valueIndex] = text[i];
					++valueIndex;
				}
				else if (isSection)
				{
					++errorRow;
					return errorRow;
				}
				else
				{
					TCHAR *pTemp = text + i + 1;
					while (!(*pTemp == _T('\r') && *(pTemp + 1) == _T('\n')) && *pTemp != 0)
					{
						if (*pTemp == _T('='))
						{
							isKey = true;
							memset(key, sizeof(key), 0);

							key[0] = text[i];
							keyIndex = 1;

							break;
						}
						else if (*pTemp == _T(']'))
						{
							TCHAR *pTemp2 = pTemp + 1;
							while (!(*pTemp2 == _T('\r') && *(pTemp2 + 1) == _T('\n')) && *pTemp2 != 0)
							{
								if (*pTemp2 != _T(' '))
								{
									break;
								}

								++pTemp2;
							}

							if (*pTemp2 == 0)
							{
								++errorRow;

								return errorRow;
							}

							if ((*pTemp2 == _T('\r') && *(pTemp2 + 1) == _T('\n')))
							{
								isSection = true;
								memset(sectionName, sizeof(sectionName), 0);
								sectionIndex = 0;

								break;
							}
						}

						++pTemp;
					}

					if ((*pTemp == _T('\r') && *(pTemp + 1) == _T('\n')) || *pTemp == 0)
					{
						++errorRow;

						return errorRow;
					}
				}
			}
		}
		else if (text[i] == _T(']'))
		{
			if (!isComment)
			{
				if (isSection)
				{
					sectionName[sectionIndex] = 0;
					Section section;
					section.sectionName = sectionName;
					sectionTable.push_back(section);

					isSection = false;
					memset(sectionName, sizeof(sectionName), 0);
					sectionIndex = 0;

					TCHAR *pTemp = text + i + 1;
					while (!(*pTemp == _T('\r') && *(pTemp + 1) == _T('\n')) && *pTemp != 0)
					{
						if (*pTemp != _T(' '))
						{
							break;
						}

						++pTemp;
					}

					if (!(*pTemp == _T('\r') && *(pTemp + 1) == _T('\n')) && *pTemp != 0)
					{
						++errorRow;
						return errorRow;
					}
				}
				else if (isKey)
				{
					key[keyIndex] = text[i];
					++keyIndex;
				}
				else if (isValue)
				{
					value[valueIndex] = text[i];
					++valueIndex;
				}
				else
				{
					++errorRow;
					return errorRow;
				}
			}
		}
		else if (text[i] == _T('='))
		{
			if (!isComment)
			{
				if (isKey)
				{
					key[keyIndex] = 0;
					isKey = false;

					if (isValue)
					{
						++errorRow;
						return errorRow;
					}

					isValue = true;
				}
				else if (isSection)
				{
					sectionName[sectionIndex] = text[i];
					++sectionIndex;
				}
				else if (isValue)
				{
					value[valueIndex] = text[i];
					++valueIndex;
				}
				else
				{
					++errorRow;
					return errorRow;
				}
			}
		}
		else if ((text[i] == _T('\r') && text[i + 1] == _T('\n')))
		{
			isComment = false;

			if (isValue)
			{
				if (isSection || isKey)
				{
					++errorRow;
					return errorRow;
				}

				value[valueIndex] = 0;
				stdstr wKey(key);
				stdstr wValue(value);
				if (sectionTable.size() <= 0)
				{
					++errorRow;
					return errorRow;
				}

				sectionTable[sectionTable.size() - 1].kValueTable[wKey] = wValue;

				memset(key, sizeof(key), 0);
				keyIndex = 0;

				isValue = false;
				memset(value, sizeof(value), 0);
				valueIndex = 0;
			}
			else if (isKey || isSection)
			{
				++errorRow;
				return errorRow;
			}

			++errorRow;

			++i;
		}
		else if (text[i] == 0)
		{
			if (isValue && !isKey && !isSection)
			{
				value[valueIndex] = 0;
				stdstr wKey(key);
				stdstr wValue(value);
				if (sectionTable.size() <= 0)
				{
					++errorRow;
					return errorRow;
				}

				sectionTable[sectionTable.size() - 1].kValueTable[wKey] = wValue;
			}
			else if (isKey)
			{
				++errorRow;
				return errorRow;
			}
		}
		else if (isSection)
		{
			if (text[i] != _T(' '))
			{
				if (isKey || isValue)
				{
					++errorRow;
					return errorRow;
				}
				else
				{
					sectionName[sectionIndex] = text[i];
					++sectionIndex;
				}
			}
		}
		else if (isKey)
		{
			if (text[i] != _T(' '))
			{
				if (isSection || isValue)
				{
					++errorRow;
					return errorRow;
				}
				else
				{
					key[keyIndex] = text[i];
					++keyIndex;
				}
			}
		}
		else if (isValue)
		{
			if (text[i] != _T(' '))
			{
				if (isSection || isKey)
				{
					++errorRow;
					return errorRow;
				}
				else
				{
					value[valueIndex] = text[i];
					++valueIndex;
				}
			}
		}
		else
		{
			if (text[i] == _T(';'))
			{
				isComment = true;
			}

			if (text[i] != _T(' ') && text[i] != 0 && !isComment)
			{
				isKey = true;

				key[keyIndex] = text[i];
				++keyIndex;
			}
		}
	}

	if (isValue)
	{
		value[valueIndex] = 0;
		stdstr wKey(key);
		stdstr wValue(value);
		if (sectionTable.size() <= 0)
		{
			++errorRow;
			return errorRow;
		}

		sectionTable[sectionTable.size() - 1].kValueTable[wKey] = wValue;
	}

	if (sectionTable.size() <= 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

stdstr CIni::ToString()
{
	stdstr context;

	// 遍历节表
	for (auto it = m_sectionTable.begin(); it != m_sectionTable.end(); ++it)
	{
		context += _T("    [");
		context += it->sectionName;
		context += _T("]\n");

		// 遍历键值表
		for (auto i = it->kValueTable.begin(); i != it->kValueTable.end(); ++i)
		{
			context += _T("    |-- ");
			context += i->first;
			context += _T(" = ");
			context += i->second;
			context += _T("\n");
		}

		context += _T("\n");
	}

	return context;
}
