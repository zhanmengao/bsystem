#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

static const std::string rootPath = "F:/Space/Windows/Windup2/";
int main()
{
	//������̨�ı�׼��������ĳɴ��ļ��ж�ȡд��
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
	/* ���in.txt�������Ӻ��exe��Ŀ¼����Ҫָ��·����D:\in.txt */
	if (freopen((rootPath + "out.txt").c_str(), "w", stdout) == NULL) /*ͬ��*/
	{
		fprintf(stderr, "error redirecting out.txt\n");
	}
	while (scanf("%d%d", &a, &b) != EOF)
		printf("%d\n", a + b);
	fclose(stdin);
	fclose(stdout);

	//�ָ�
	//�ص�������̨�ı�׼������� windowsΪ"CON" linuxΪ"/dev/console"
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	printf("Finish ����0��ʾ����:\n");
	int n = 0;
	do
	{
		scanf("%d", &n);
	} 
	while (n != 0);

	return 0;
}