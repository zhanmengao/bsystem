#pragma once
// ��
class A
{
private:
	const int a;                // �������Ա��ֻ���ڳ�ʼ���б�ֵ

public:
	// ���캯��
	A() : a(0) { };
	A(int x) : a(x) { };        // ��ʼ���б�

								// const�����ڶ����غ���������
	int getValue();             // ��ͨ��Ա����
	int getValue() const;       // ����Ա�����������޸����е��κ����ݳ�Ա��ֵ
};

void function()
{
	// ����
	A b;                        // ��ͨ���󣬿��Ե���ȫ����Ա���������³���Ա����
	const A a;                  // ������ֻ�ܵ��ó���Ա����
	const A *p = &a;            // ָ�������ָ�򳣶���
	const A &q = a;             // ָ�򳣶��������

	// ָ��
	char greeting[] = "Hello";
	char* p1 = greeting;                // ָ�������ָ���ַ��������
	const char* p2 = greeting;          // ָ�������ָ���ַ����鳣����const ������ char��˵��ָ����ַ���char�����ɸı䣩
	char* const p3 = greeting;          // �����ǳ�����ָ�룬ָ���ַ����������const ������ p3��˵�� p3 ָ�������ɸı䣩
	const char* const p4 = greeting;    // �����ǳ�����ָ�룬ָ���ַ����鳣��
}

// ����
void function1(const int Var);           // ���ݹ����Ĳ����ں����ڲ��ɱ�
void function2(const char* Var);         // ����ָ����ָ����Ϊ����
void function3(char* const Var);         // ����ָ��Ϊ����
void function4(const int& Var);          // ���ò����ں�����Ϊ����

// ��������ֵ
const int function5();      // ����һ������
const int* function6();     // ����һ��ָ������ָ�������ʹ�ã�const int *p = function6();
int* const function7();     // ����һ��ָ������ĳ�ָ�룬ʹ�ã�int* const p = function7();