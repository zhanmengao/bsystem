#pragma once
#include <atlstr.h>
#include <string>

using namespace std;

typedef struct _UserStruct
{

	CString Name;
	CString Comment;
	CString HDrive;
	CString HomeDirPath;
	CString ScriptPath;
	CString ProfilePath;

}UserStruct_I1, *PUserStruct_I1;

typedef struct Filepath
{
	string names;
	int    types;
	_fsize_t    size;
	string path;
	__time64_t time;
};


typedef struct FileName {//�洢��ʽ��"�ļ���.��չ��"
	char Fname[64];
	int len;
};