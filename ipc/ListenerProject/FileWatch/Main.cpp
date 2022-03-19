#include "Logic.h"
#include "Tools.h"

int main()
{
	LoadConfig();
	StartThread();
	fileWatcher();
	return 0;
}

