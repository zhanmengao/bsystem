#include"WinStrUtils.h"
#include<stdio.h>
#include"StrUtils.h"
void main1()
{
	TCHAR* s = L"QWQQAQ";
	TCHAR* t1 = L"QAQ";
	TCHAR* t2 = L"Q";
	TCHAR* t3 = L"Z";
	TCHAR* t4 = L"QZ";
	TCHAR* t5 = L"qaq";
	printf("t1 %d \n", WinStrUtils::Find(s, t1));
	printf("t2 %d \n", WinStrUtils::Find(s, t2));
	printf("t3 %d \n", WinStrUtils::Find(s, t3));
	printf("t4 %d \n", WinStrUtils::Find(s, t4));
	printf("t5 %d \n", WinStrUtils::Find(s, t5));
	printf("t5 %d \n", WinStrUtils::Findi(s, t5));

	printf("%d \n", Find("aaabbbc", "bb"));
	printf("%d \n", Count("aaabbbc", strlen("aaabbbc"), "bb"));
	char buf[64] = { "aaabbbc" };
	replaceAll(buf, sizeof(buf), "bb", "ccCCC", NULL);
	printf("%s \n", buf);

#define URL "www.baidu.com/20200908 11"
	std::string url = StrUtils::UrlEncode(URL);
	printf("%s \n", url.c_str());
	printf("%s \n", StrUtils::UrlDecode(url).c_str());

	char encode[1024] = { 0 };
	URLEncode(URL, strlen(URL), encode, 1024);
	printf("%s \n", encode);
	char decode[1024] = { 0 };
	URLDecode(encode, strlen(encode), decode, 1024);
	printf("%s \n", decode);

#define URL "http://192.168.50.212/c-api/user-login-pwd"

	stdstr wstr(TEXT(URL));
	vector<stdstr> sv;
	StrUtils::Split(wstr, sv, L"://");
	for (auto it = sv.begin(); it != sv.end(); it++)
	{
		printf("%ws \n", it->c_str());
	}
}

void main()
{
#define PATH "D:\\notepad\\Notepad++\\notepad++.exe"
	std::vector<stdstr> vec;
	StrUtils::ReplaceAllW(_T(PATH), '\\', '/');
	stdstr remoteapp(_T(PATH));
	StrUtils::Split(remoteapp, vec, '/');
	if (vec.size() > 0)
	{
		remoteapp = vec[vec.size() - 1];
	}

	//如果末4位是.exe
	size_t pos = remoteapp.find_last_of(_T(".exe"));
	cout << remoteapp.size()<<endl<< pos << endl;
}