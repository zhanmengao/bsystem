

int count = 11;         // ȫ�֣�::���� count

class A 
{
public:
	static int count;   // �� A �� count��A::count��
};
int A::count = 21;

void fun()
{
	int count = 31;     // ��ʼ���ֲ��� count Ϊ 31
	count = 32;         // ���þֲ��� count ��ֵΪ 32
}

int main222() 
{
	::count = 12;       // ���� 1������ȫ�ֵ� count ��ֵΪ 12

	A::count = 22;      // ���� 2�������� A �� count Ϊ 22

	fun();        // ���� 3

	return 0;
}