#include<iostream>
#include <string>
using namespace std;

void Division(int n)
{
	if (n < 2 || n>79)
	{
		return;
	}
	//蛮力法
	for (int j = 0; j <= 9; j++)
	{
		for (int i = 0; i <= 9; i++)
		{
			if (i == j)
			{
				continue;
			}
			for (int h = 0; h <= 9; h++)
			{
				if (h == i || h==j)
				{
					continue;
				}	
				for (int g = 0; g <= 9; g++)
				{
					if (g == h || g==i ||g==j)
					{
						continue;
					}
					for (int f = 0; f <= 9; f++)
					{
						if (f == h || f == i || f == j|| f==g)
						{
							continue;
						}
						int val2 = f * 10000 + g * 1000 + h * 100 + i* 10 + j;
						int val1 = val2*n;
						if (val1 > 98765)
						{
							continue;
						}
						cout << "val1=" << val1 << "val2="<<val2<<endl;

						//计算val1的每个数字
						int a = val1 / 10000;
						int b = val1 / 1000 % 10;
						int c = val1 / 100 % 10;
						int d = val1 / 10 % 10;
						int e = val1 % 10;
						if (a != f && a != g && a != h && a != i && a != j)
						{
							if (b != f && b != b && b != h && b != i && b != j &&b != a)
							{
								if (c != f && c != g && c != h && c != i && c != j &&c != a &&c != a)
								{
									if (d != f && d != g && d != h && d != i && d != j &&d != a &&d != a &&d != c)
									{
										if (e != f && e != g && e != h && e != i && e != j &&e != a &&e != a &&e != c &&e != d)
										{
											cout << val1 << "/" << val2 << "="<<n<<endl;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
