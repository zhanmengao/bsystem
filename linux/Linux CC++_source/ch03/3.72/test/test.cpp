#include<iostream>
#include<string>

using namespace std;

int main()
{
    cout<<"find test"<<endl;
    //����size_type find (charT c, size_type pos = 0) const noexcept;
    string st1("babbabab");
    cout << st1.find('a') << endl;//1   ��ԭ��֪����ʡ�Ե�2����������Ĭ�ϴ�λ��0������1���ַ�����ʼ����
    cout << st1.find('a', 0) << endl;//1
    cout << st1.find('a', 1) << endl;//1   
    cout << st1.find('a', 2) << endl;//4   ��st1�У���λ��2��b������λ��2����ʼ�������ַ�a�������״�ƥ���λ�ã���ƥ��ʧ�ܣ�����npos
    cout << st1.rfind('a',7) << endl;//6   ����rfind�����潲��
    cout << st1.find('c', 0) << endl;//4294967295
    cout << (st1.find('c', 0) == -1) << endl;//1
    cout << (st1.find('c', 0) == 4294967295) << endl;//1   ��������1��ԭ���Ǽ������-1��4294967295����ʾΪ32��1�������ƣ�
    cout << st1.find('a', 100) << endl;//4294967295   �����ҵ���ʼλ�ó����ַ�������ʱ��������ʧ�ܴ�������npos
    //����size_type find (const basic_string& str, size_type pos = 0) const noexcept;
    string st2("aabcbcabcbabcc");
    string str1("abc");
    cout << st2.find(str1, 2) << endl;//6   ��st2��λ��2��b����ʼƥ�䣬���ص�һ�γɹ�ƥ��ʱƥ��Ĵ���abc�������ַ���st2�е�λ�ã�ʧ�ܷ���npos
    //����size_type find (const charT* s, size_type pos = 0) const;
    cout << st2.find("abc", 2) << endl; //6   ͬ�ϣ�ֻ������������string����char*
    //����size_type find (const charT* s, size_type pos, size_type n) const;
    cout << st2.find("abcdefg", 2, 3) << endl;//6   ȡabcdefg��ǰ3���ַ���abc������ƥ�䣬�൱��st2.find("abc", 2)
    cout << st2.find("abcbc", 0, 5) << endl;//1   �൱��st2.find("abcbc", 0)
    cout << st2.find("abcbc", 0, 6) << endl;//4294967295   ��3������������1�������ĳ���ʱ������npos
    return 0;
}