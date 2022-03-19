#include<iostream>
#include <string>
#include <iomanip>
using namespace std;

void main1()
{
	int i;
	cin >> i;
	
	cout << "以左对齐方式输出整数，域宽为12，请输入您要显示的数"<<endl;
	cin >> i;
	cout << setiosflags(ios::left)<<setw(12)<<i<<endl;
	cout << "以八进制，十进制，十六进制输出整数"<<endl;
	cin >> i;
	cout << "十进制"<<dec << i<<endl;;
	cout<<"16进制"<<hex<<i<<endl;
	cout<<"8进制"<<oct<<i<<endl;
	cout << "实现浮点数的指数格式和顶点格式的输入/输出 并指定精度"<<endl;
	cout << ios::scientific << setprecision(2) << i << endl;
	cout << ios::fixed << setprecision(2) << i << endl;


}