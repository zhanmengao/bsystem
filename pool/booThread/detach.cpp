#include "Define.h"


/*
detach()�����̺߳͸��̷߳��롣
�����̺߳󣬿��Ա����쳣��ȫ���⣬��ʹ�߳����ں�̨���У��������Ҳ��ȷ��std::terminate��std::thread��������ʱ�����á�
ͨ���Ʒ����߳�Ϊ�ػ��̣߳�deamon threads���������̵߳��ص���ǳ�ʱ�����У�
�̵߳��������ڿ��ܻ��ĳһ��Ӧ����ʼ�����������ܻ��ں�̨�����ļ�ϵͳ�����п��ܶԻ�����������������ݽṹ�����Ż���
*/
int maindetach()
{
	std::thread t([](std::string text)
	{
		std::cout << "hello world, " << text << std::endl;
	}, "lee");

	if (t.joinable())
	{
		//����
		t.detach();
	}
	assert(!t.joinable());

	return 0;
}