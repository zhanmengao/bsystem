#include<iostream>
#include<string>

using namespace std;

int main()
{
    //����size_type find_first_not_of (const charT* s, size_type pos = 0) const;
    string str("abcdefg");
    cout << str.find_first_not_of("kiajbvehfgmlc", 0) << endl;//3   ��Դ��str��λ��0(a)��ʼ���ң�Ŀ�괮����a��ƥ�䣩������b��bƥ��,����c��cƥ�䣬
//    ����d��Ŀ�괮��û��d����ƥ�䣩��ֹͣ���ң�����d��str�е�λ��3
    return 0;
}

