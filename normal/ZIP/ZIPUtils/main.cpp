#include"ZIPUtils.h"
#include"../../File/FileManager/FileManager/PathHelper.hpp"

void main()
{
#define FILE_NAME "ZLIBUtils.zip"
	UnzipFile((PathHelper::GetModuleFolder() + FILE_NAME), (PathHelper::GetModuleFolder() + "C"));

}