#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	int nNum = 0x12345678;
	char *p = (char*)&nNum;  //pָ��洢nNum���ڴ�ĵ͵�ַ
	 
	if (*p == 0x12) cout << "This machine is big endian." << endl; //�жϵ͵�ַ�Ƿ��ŵ������ݸ�λ
	else cout << "This machine is small endian." << endl;   
  
	return 0;
}
