#include "string.h"
#include "iostream"
using namespace std;

int main(int argc, char* argv[])
{
	char sz1[] = "a����a1a��ȥasdfad����";
	string str;
	int i, len = strlen(sz1); //�õ��ַ����鳤��

	for (int i = 0; i < len;)
	{
		if (sz1[i] < 0)   //������ǰ�������ֽڴ���Ǻ���
		{
			str.push_back(sz1[i]); 
			i++; 
			str.push_back(sz1[i]);
		}
		i++;
	}
	cout << str << endl; //����ҵ��ĺ���

	return 0;
}
