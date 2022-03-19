#ifndef _STRING_HPP
#define _STRING_HPP
#include<string.h>
#include<stdlib.h>
#include"CString.h"

#ifdef __cplusplus
#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
#include <iostream>
#include<memory>

using namespace std;

namespace stb
{
	class  String final
	{
		friend ostream& operator<<(ostream &out, String &s);
		friend istream& operator >> (istream &in, String &s);
	public:
		String(unsigned len = 0)
		{
			if (len == 0)
			{
				m_len = 0;
				m_p = new char[m_len + 1];
				strcpy(m_p, "");
			}
			else
			{
				m_len = len;
				m_p = new char[m_len + 1];
				memset(m_p, 0, m_len);
			}
		}
		String(const char *p)
		{
			if (p == NULL)
			{
				m_len = 0;
				m_p = new char[m_len + 1];
				strcpy(m_p, "");
			}
			else
			{
				m_len = strlen(p);
				m_p = new char[m_len + 1];
				strcpy(m_p, p);
			}
		}
		String(const String& s)
		{
			m_len = s.m_len;
			m_p = new char[m_len + 1];
			strcpy(m_p, s.m_p);
		}
		~String()
		{
			Clear();
		}
	public:
		void Clear()
		{
			if (m_p != NULL)
			{
				delete[] m_p;
				m_p = NULL;
				m_len = 0;
			}
		}
		String substr(unsigned nBegin, unsigned nSize)
		{
			if (nBegin + nSize > m_len)
			{
				return nullptr;
			}
			String str(nSize);
            strcpy(str.m_p, str.m_p + nBegin);
			str[nSize] = '\0';
			return str;
		}
		void Replace(const char* src, const char *target)
		{
			struct cstring str = { 0 };
			String_InitStr(&str, m_p, m_len);
			String_ReplaceAllStr(&str, src, target);
			String temp;
			temp.m_p = m_p;
			m_p = str.string;
			m_len = str.nSize;
		}

		int Find(const String& model, unsigned beginPos = 0)
		{
			unsigned *nextArr = new unsigned[model.Length() + 1];
			KMP_NextArr(model, nextArr);
			unsigned modelndex = 0;
			while (beginPos < m_len)
			{
				if (m_p[beginPos] == model[modelndex] || modelndex == 0)
				{
					beginPos++;
					modelndex++;
					if (modelndex == model.Length())
					{
						return beginPos - modelndex + 1;
					}
				}
				else
				{
					modelndex = nextArr[modelndex];
				}
			}
			if (nextArr != nullptr)
			{
				delete[] nextArr;
				nextArr = nullptr;
			}
			return -1;
		}
	private:
		void KMP_NextArr(const String& model, unsigned *nextArr)
		{
			memset(nextArr, 0, sizeof(unsigned)*model.Length());
			unsigned b = 0;
			unsigned cur = 1;
			while (cur < model.Length())
			{
				if (b == 0 || model[b] == model[cur])
				{
					b++;
					cur++;
					nextArr[cur] = b;
				}
				else
				{
					b = nextArr[b];
				}
			}
		}
	public:
		String operator+(const String& s)
		{
            if(s.Length()<=0)
            {
                return *this;
            }
			String ret(m_len + s.Length() + 1);
			strcpy(ret.m_p, m_p);
			strcpy(ret.m_p + m_len, s.m_p);
			return ret;
		}
		String& operator+=(const String& s)
		{
			if (s.Length() > 0)
			{
				String str(m_len + s.Length() + 1);
				strcpy(str.m_p, m_p);
				strcpy(str.m_p + m_len, s.m_p);

				this->m_p = s.m_p;
				m_len = str.m_len;
			}
			return *this;
		}
	public:
		String& operator=(const char *p)
		{
            if(p == NULL || strlen(p) == 0)
            {
                Clear();
                return *this;
            }
			if (strcmp(m_p, p) != 0)
			{
				String str(p);
				char* pTemp = str.m_p;
				str.m_p = m_p;
				m_p = pTemp;
			}
			return *this;
		}
		String& operator=(const String &s)
		{
			if (*this != s)
			{
				String str(s);
				char* pTemp = str.m_p;
				str.m_p = m_p;
				m_p = pTemp;
			}
			return *this;
		}
		char& operator[] (unsigned index)
		{
			return m_p[index];
		}
		const char& operator[] (unsigned index) const
		{
			return m_p[index];
		}

	public:
		bool operator==(const char *p) const
		{
			if (p == NULL)
			{
				if (m_len == 0)
				{
					return true;
				}
			}
			else 	if (m_len == strlen(p))
			{
				return !strcmp(m_p, p);
			}
			return false;
		}
		bool operator==(const String& s) const
		{
			if (m_len != s.m_len)
			{
				return false;
			}
			return !strcmp(m_p, s.m_p);
		}
		bool operator!=(const char *p) const
		{
			return !(*this == p);
		}
		bool operator!=(const String& s) const
		{
			return !(*this == s);
		}

	public:
		int operator<(const char *p)
		{
			return strcmp(this->m_p, p);
		}
		int operator>(const char *p)
		{
			return strcmp(p, this->m_p);
		}
		int operator<(const String& s)
		{
			return strcmp(this->m_p, s.m_p);
		}
		int operator>(const String& s)
		{
			return  strcmp(s.m_p, m_p);
		}
	public:
		const char *c_str() const
		{
			return m_p;
		}
		int Length() const
		{
			return m_len;
		}
	private:
		unsigned		m_len;
		char	 *m_p;

	};
	ostream& operator<<(ostream &out, String &s)
	{
		out << s.m_p;
		return out;
	}
	istream& operator >> (istream &in, String &s)
	{
		cin >> s.m_p;
		return in;
	}

	void StringMain()
	{
		String s1;
		String s2("s2");
		String s2_2 = nullptr;
		String s3 = s2;
		String s4 = "s4444444444";

		//测试运算符重载 和 重载[]
		//=

		s4 = s2;
		s4 = "s2222";
		s4[1] = '4';
		printf("%c", s4[1]);

		cout << "s4:" << s4 << endl;
		//ostream& operator<<(ostream &out, String &s)

		//char& operator[] (int index)
		//String& operator=(const char *p);
		//String& operator=(const String &s);

		if (s2 == "aa")
		{
			printf("s2 == aa\n");
		}
		else
		{
			printf("s2 != aa\n");
		}

		if (s3 == s2)
		{
			printf("s3 == s2\n");
		}
		else
		{
			printf("s3 != s2\n");
		}

		s3 = "aaa";

		int tag = (s3 < "bbbb");
		if (tag < 0)
		{
			printf("s3 小于 bbbb\n");
		}
		else
		{
			printf("s3 大于 bbbb\n");
		}

		String s5 = "aaaaffff";
		s5 = "aa111";
		cout << s5 << endl;

		String s6(128);
		cout << "\n请输入字符串(回车结束)";
		cin >> s6;

		cout << s6 << endl;

		String fu = "babcababcaabcabx";
		String zi = "abcabx";
		cout << "String Find:" << fu.Find(zi) << endl;

		{
			const char* url = "http://typdm.tydevice.com/?jsonstr="
				"{\"VER\":\"01\""
				",\"CTEI\":\"CT1234561234567\""
				",\"MAC\":\"24:e2:71:f4:d7:b0\""
				",\"IP\":\"192.168.0.1\","
				"\"UPLINKMAC\":\"01:02:03:04:05:06\","
				"\"LINK\":\"1\",\"FWVER\":\"1.1\",\"DATE\":\"2018-07-01 12:05:30\"}";
			String str(url);
			str.Replace(" ", "%20");
			cout << "str.Replace" << str << endl;

			struct cstring cstr = { 0 };
			String_InitStr(&cstr, url,strlen(url));
			String_UrlEncode(&cstr);
			printf("%s \n", cstr.string);
		}

	}
}
#endif
#endif
