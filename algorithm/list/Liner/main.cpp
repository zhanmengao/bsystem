#include"../stb/array.hpp"
#include"../Queue/Stack.h"
#include"../Queue/Queue.h"
#include"../stb/BinaryTree.h"
#include"../stb/Bitmap.h"
#include<string>
using namespace stb;
using namespace std;


void mainArr()
{
	Array<int>  a1(10);

	for (int i = 0; i < a1.Length(); i++)
	{
		a1[i] = i;
	}

	cout << "\n��ӡ����a1: ";
	for (int i = 0; i < a1.Length(); i++)
	{
		cout << a1[i] << "	";
	}
	cout << endl;

	Array<int> a2 = a1;
	cout << "\n��ӡ����a2: ";
	for (int i = 0; i < a2.Length(); i++)
	{
		cout << a2[i] << " ";
	}
	cout << endl;

	//3
	Array<int> a3(5);
	{
		a3 = a1;
		a3 = a2 = a1;
		cout << "\n��ӡ����a3: ";
		for (int i = 0; i < a3.Length(); i++)
		{
			cout << a3[i] << " ";
		}
	}


	//����4
	if (a3 == a1)
	{
		printf("a3 a1���\n");
	}
	else
	{
		printf("a3 a1�����\n");
	}

	Array<string> mStrArr(10);
	for (int i = 0; i < mStrArr.Length(); i++)
	{
		mStrArr[i] = "q";
	}
	Array<string> mStrArr2(mStrArr);
	for (int i = 0; i < mStrArr2.Length(); i++)
	{
		cout << mStrArr2[i] << " ";
	}
}


int main()
{
	return 0;
}