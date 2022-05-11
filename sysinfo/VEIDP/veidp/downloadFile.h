#pragma once
//#include <Windows.h>
//обть
#include <cstring>
#include <windows.h>
#include <urlmon.h>
#include <string>
#include <vector>
#include <algorithm>
#include "infoCollect.h"
#pragma comment(lib, "urlmon.lib")
#include "Wincrypt.h"

using namespace std;

#include "json/json.h"


void Download(int instruct, bool ifSortSent, const string& reader);//обть
