#include<iostream>
#include<string>

using namespace std;

int main()
{
	cout<<"find_last_of test"<<endl;
    //����size_type find_last_of (const charT* s, size_type pos = npos) const;
    //Ŀ�괮�н����ַ�c��Դ���е�����cƥ�䣬�����ַ�����ƥ��
    string str("abcdecg");
    cout << str.find_last_of("hjlywkcipn", 6) << endl;//5   ��str��λ��6(g)��ʼ��ǰ�ң�g��ƥ�䣬����c��cƥ�䣬ֹͣ���ң�����c��str�е�λ��5
    cout << str.find_last_of("hjlywkcipn", 4) << endl;//2   ��str��λ��4(e)��ʼ��ǰ�ң�e��ƥ�䣬����d,d��ƥ�䣬����c��cƥ�䣬ֹͣ���ң�
                                                      //    ����c��str�е�λ��5
    cout << str.find_last_of("hjlywkcipn", 200) << endl;//5   ����2����������Դ���ĳ��ȣ�����str������7��ʱ����������൱�ڴ�Դ�������һ
                                                        //    ���ַ���ʼ����
    return 0;
}