#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

static const std::string rootPath = "F:/Space/Windows/Windup2/";
int main()
{
	//将控制台的标准输入输出改成从文件中读取写入
	/* redirect standard output to a file */
	if (freopen((rootPath + "outfile.txt").c_str(), "w", stdout) == NULL)
		fprintf(stderr, "error redirecting stdout\n");
	/* this output will go to a file */
	printf("This will go into a file.\n");
	/*close the standard output stream*/
	fclose(stdout);

	int i;
	if (freopen((rootPath+"output.txt").c_str(), "w", stdout) == NULL)
		fprintf(stderr, "error redirecting stdout\n");
	for (i = 0; i < 10; i++)
		printf("%3d", i);
	printf("\n");
	fclose(stdout);

	int a, b;
	if (freopen((rootPath + "in.txt").c_str(), "r", stdin) == NULL)
	{
		fprintf(stderr, "error redirecting in.txt\n");
	}
	/* 如果in.txt不在连接后的exe的目录，需要指定路径如D:\in.txt */
	if (freopen((rootPath + "out.txt").c_str(), "w", stdout) == NULL) /*同上*/
	{
		fprintf(stderr, "error redirecting out.txt\n");
	}
	while (scanf("%d%d", &a, &b) != EOF)
		printf("%d\n", a + b);
	fclose(stdin);
	fclose(stdout);

	//恢复
	//回到到控制台的标准输入输出 windows为"CON" linux为"/dev/console"
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	printf("Finish 输入0表示结束:\n");
	int n = 0;
	do
	{
		scanf("%d", &n);
	} 
	while (n != 0);

	return 0;
}