#include"TableManager.h"

int main()
{
	TableManager table;
	table.Load("D:/Space/Game/Card/Client/BossShow/BossShow.txt");
	auto tab = table.GetData();
	const DATA* pData = table.GetDataByID(1);
	std::string tips = table.GetDataByID(1, "Tips");
	return 0;
}