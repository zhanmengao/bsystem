#include "Define.h"
#ifdef __linux
#include<unistd.h>
#define Sleep(n) sleep(n/1000)
#else
#include <Windows.h>
#endif
/*
Ĭ������£���ʹ�����̺߳����Ĳ������������ͣ��������ȱ��������߳̿ռ䣬Ȼ���߳�ִ������ʡ�������߳̿ռ�Ϊ�߳��ܹ����ʵ��ڲ��ڴ档
*/
void f(int i, std::string const& s)
{
	cout << i << "call,str = " << s << endl;
}

//��ʹf�ĵڶ����������������ͣ��ַ�������ֵ"hello"���Ǳ��������߳�t�ռ��ڣ�Ȼ��ת��Ϊstd::string���͡�
void oopsConst(int some_param)
{
    std::thread t(std::ref(f), some_param, "hello");
	t.detach();
}
//�������������ֲ���Ϊָ���Զ�������ָ�������¾ͺ����׳�������������£�ָ�����buffer���ᱻ�������߳�t�ռ��ڣ�
//���ʱ��ܿ��ܺ���oopsError�����ˣ�buffer��û�б�ת��Ϊstd::string���̺߳����õ��ľ���һ���Ѿ���������ջ����
void oopsError(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
    std::thread t(std::ref(f), some_param, buffer);           //��ʽת�������̺߳��������òŷ������죬����ȫ��
	t.detach();
}
void not_oops(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
    std::thread t(std::ref(f), some_param, std::string(buffer));           //�ֶ�ת������������ȥ�ľ���string��������ȫ
	t.detach();
}

/*
����������˵�����̴���ʱ���������ᱻ����һ�ο�����
���Լ�ʹ���ǽ����̺���������Ϊ���ã�Ҳֻ�Ƕ���ݿ��������ã�����ԭ���ݵ�����
���ǶԲ����Ĳ���������ı��䴫��֮ǰ��ֵ��
*/
void display_status(int &w,string& data)
{
	cout << "w.path = " << &w << "      w =" << w << endl;
	cout <<"data.path = "<< &data << "     data ="<< data<<endl;
}
//���������������ýӵ���ֻ�ǿ���������
void PthreadMain(int &w, string& data)
{
	cout << "child------------------------------------"<<endl;
	display_status(w, data);
	w++;
	data = "222222222";
	Sleep(5000);
}
void oops_error_ref(int w)
{
	string data = "father";
    std::thread t(std::ref(PthreadMain), w, data);         //data����������������ԭ���ı���
	t.join();
	data += to_string(w);
	cout << "oops_error_ref------------------------------------" << endl;
	display_status(w, data);
}

//�����������std::ref��������
void oops_ref(int w)
{
	string data = "father";
    std::thread t(std::ref(PthreadMain), std::ref(w), std::ref(data));
	t.join();
	cout << "oops_ref------------------------------------" << endl;
	display_status(w, data);
}

/*
���ڿ����ƶ����ɿ����Ĳ�����Ʃ��std::unqiue_ptr�������Դ��������ʱ�ģ��ƶ��������Զ�ִ�еģ�
���Դ����������������������ʽ����std::move������
*/
void PthreadMovePtr(int &w, std::unique_ptr<string>& data)
{
	cout << "child------------------------------------" << endl;
	display_status(w, *data);
	w++;
	*data = "222222222";
	Sleep(5000);
}
void oops_move(int w)
{
	std::unique_ptr<string> p(new string);
	*p = "o00";
    std::thread t(std::ref(PthreadMovePtr), w, std::move(p));
	t.join();
	cout << "oops_move------------------------------------" << endl;
	display_status(w, *p);       //�˿�p�Ѿ���ת������
}

void mainPara()
{
	oopsConst(1);
	oopsError(2);
	not_oops(3);
	Sleep(3000);
	oops_error_ref(4);
	cout << endl;
	Sleep(1000);
	oops_ref(5);
	cout << endl;
	Sleep(1000);
	oops_move(6);
}
