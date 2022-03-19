
#include <iostream>
using namespace std;

// ����ģ�� ���� : ���Ͳ�����  
//����.....

namespace demo2{
	//������ַ�����  int�����������

	template <typename T, typename T2 >
	int mySort(T *array, T2 size)
	{
		T2 i, j;
		T tmp;
		if (array == NULL)
		{
			return -1;
		}

		//ѡ��  
		for (i = 0; i < size; i++)
		{
			for (j = i + 1; j < size; j++)
			{
				if (array[i] < array[j])
				{
					tmp = array[i];
					array[i] = array[j];
					array[j] = tmp;
				}
			}
		}
		return 0;
	}

	template <typename T, typename T2>
	int myPrint(T *array, T2 size)
	{
		T2 i = 0;
		for (i = 0; i < size; i++)
		{
			cout << array[i] << " ";
		}
		return 0;
	}
}
using namespace demo2;
void main201()
{
	// int����
	{
		int myarray[] = { 11, 33, 44, 33, 22, 2, 3, 6, 9 };

		int size = sizeof(myarray) / sizeof(*myarray);
		mySort<int, int>(myarray, size);

		printf("����֮��\n");
		myPrint(myarray, size);

	}

	//char ����
	{
		char buf[] = "aff32ff2232fffffdssss";
		int len = strlen(buf);

		mySort<char, int>(buf, len);
		myPrint<char, int>(buf, len);

	}
}
