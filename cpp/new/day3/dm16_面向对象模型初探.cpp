#include "iostream"

using namespace std;

class C1
{
public:
	int i;  //4
	int j; //4
	int k;  //4
protected:
private:
}; //12

class C2
{
public:
	int i; //4
	int j; //4
	int k; //4

	static int m; 
public:
	int getK() const { return k; } 
	void setK(int val) { k = val; }  

protected:
private:
}; //12

struct S1
{
	int i;
	int j;
	int k;
}; //12

struct S2
{
	int i;
	int j;
	int k;
	static int m;
}; //12

//静态变量会放在虚拟内存的 数据区（data段），而不占用变量的存储
int main1601()
{
	printf("c1:%d \n", sizeof(C1));
	printf("c2:%d \n", sizeof(C2));
	printf("s1:%d \n", sizeof(S1));
	printf("s2:%d \n", sizeof(S2));
	
	system("pause");
	return 0;
}
