#include"CommandCall.h"
void Read(const std::string& rData,void* argv)
{
	cout << rData << endl;
}
int main()
{
	CommandCall("Install.txt", Read);
	ScriptItem item;
	item.Script = "D:\\Space\\System\\SystemInfo\\SystemInfo\\Debug\\HardwareUtils.exe";
	CommandCall(item, Read);

	item.Script = "D:\\Space\\Network\\NetDisk\\NetDiskUtils\\Release\\NetDiskUtils.exe";
	item.Command = "Z: http://192.168.50.44/remote.php/dav/files/admin/ admin 123456";
	CommandCall(item, Read);

	item.Script = "";
	item.Command = "D:\\Space\\Network\\NetDisk\\NetDiskUtils\\Release\\NetDiskUtils.exe Z: http://192.168.50.44/remote.php/dav/files/admin/ admin 123456";
	CommandCall(item, Read);
	return 0;
}