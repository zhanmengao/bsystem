#pragma  once

#include "Definer.h"
#include <iostream>
#include <fstream>

bool LoadConfig();
bool SaveDataToFile(const std::string& szFilePath, const std::string& text);
wstring string2wstring(const string& str);