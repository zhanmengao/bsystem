#include"PowerOff.h"


int main(void)
{
	CHAR ch;
	printf(">>>>>>>>>>>>>>>>>>>>>> Demo >>>>>>>>>>>>>>>>>\n*\n");
	printf("* 1. Power_Off\n*\n");
	printf("* 2. Log_Off\n*\n");
	printf("* 3. ReBoot\n*\n");
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	ch = getch();
	while (1)
	{
		switch (ch)
		{
		case '1':
			if (!powerOffProc())
			{
				printf("Process Error!\n");
				continue;
			}
			return 0;
		case '2':
			if (!logOffProc())
			{
				printf("Process Error!\n");
				continue;
			}
			return 0;
		case '3':
			if (!reBootProc())
			{
				printf("Process Error!\n");
				continue;
			}
			return 0;
		default:
			printf("Error!\n");
		}
	}
	system("pause");
	return 0;
}