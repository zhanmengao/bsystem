#include<iostream>
#include<string>

using namespace std;

int main()
{
	cout<<"find_first_of test:"<<endl;
    //����size_type find_first_of (charT c, size_type pos = 0) const noexcept;
    string str("babccbabcc");
    cout << str.find('a', 0) << endl;//1
    cout << str.find_first_of('a', 0) << endl;//1   str.find_first_of('a', 0)��str.find('a', 0)
    //����size_type find_first_of (const basic_string& str, size_type pos = 0) const noexcept;
    string str1("bcgjhikl");
    string str2("kghlj");
    cout << str1.find_first_of(str2, 0) << endl;//��str1�ĵ�0���ַ�b��ʼ�ң�b����str2�е������ַ�ƥ�䣻����c,c����str2�е������ַ�ƥ�䣻����g��
                                                //g��str2�е�gƥ�䣬����ֹͣ���ң�����g��str1�е�λ��2
    //����size_type find_first_of (const charT* s, size_type pos, size_type n) const;
    cout << str1.find_first_of("kghlj", 0, 20);//2   ���ܵ�3������������kghlj�ĳ��ȣ������ܵõ���ȷ�Ľ����������Ϊ��str1�Ǻ�"kghlj+����"��ƥ��
    return 0;
}

