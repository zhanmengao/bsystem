#include<iostream>;
using namespace std;

/*��������ǳ��������
*/

class Name {
public:
	/*
	//�������캯��
	Name(Name &obj)
	{
		//���
		cout << "����Name��copy���캯��" << endl;

		//��obj����ʼ���Լ�
		this->p = (char*)malloc(obj.len + 1);
		len = obj.len;

	}
	*/
	char *p;
	int len;
	Name(const char * myp)
	{
		cout << "����Name�Ĺ��캯��" << endl;
		int len = strlen(myp);
		p = (char *)malloc(len + 1);
	}
	~Name()
	{
		cout << "����Name����������" << endl;
		if (p != NULL)
		{
			//����
			free(p);
			p = NULL;
			len = 0;
		}
	}
protected:

private:

};

int mainplayer()
{
	//����
	Name n1("abc");
	Name n2 = n1;
	//�ö���ָ�봢������һ��ָ�����ָ�򣨶�����ַ��
	char **p1 = &(n1.p);
	char **p2 = &(n2.p);
	cout << *p1 << endl;
	cout << *p2 << endl;
	//	n2 = n1;
	return 0;
}

int main22()
{
	mainplayer();
	system("pause");
	return 0;
}