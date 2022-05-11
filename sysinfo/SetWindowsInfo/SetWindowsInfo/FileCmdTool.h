#pragma once
class FileCmdTool
{
public:
	FileCmdTool();
	~FileCmdTool();
		
	static int SetSysFile2AdminACL(wchar_t * filename);
	static int FileRename(wchar_t * source, wchar_t * newname);

private:

	static int CallCmd2(wchar_t * cmdline);
	

};

