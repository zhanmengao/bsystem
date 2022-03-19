#include"Env.h"
#include"Command.h"
#include<iostream>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
int _tmain(int argc, TCHAR* argv[], TCHAR* env[])
{
	GetEnvironmentQS();
	EnvExpand();
	DumpEnvStrings();
	DumpEnvVariables(env);
	STDOUT << GetCommand() << endl;
	for (int i = 0; i < argc; i++)
	{
		STDOUT << argv[i] << endl;
	}
	auto vec = GetCommandArray();
	for (int i = 0; i < vec.size(); i++)
	{
		wcout << vec[i] << endl;
	}
	system("pause");
}