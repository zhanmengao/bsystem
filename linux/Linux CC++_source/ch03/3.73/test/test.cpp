//�ҳ��ַ���str�����е�"abc"�����λ�ã�����δ�ҵ������"not find!"
#include<iostream>
#include<string>

using namespace std;

int main()
{
    string str("babccbabcaabcccbabccabcabcabbabcc");
    int num = 0;
    size_t fi = str.find("abc", 0);    
    while (fi!=str.npos)
    {
        cout << fi << "   ";
        num++;
        fi = str.find("abc", fi + 1);
    }
    if (0 == num)
        cout << "not find!";
    cout << endl;
    return 0;
}