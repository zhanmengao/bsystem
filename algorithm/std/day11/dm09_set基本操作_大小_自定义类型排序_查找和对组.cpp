#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "set"
#include <xfunctional>
//1 ���� Ԫ��Ψһ �Զ�����(Ĭ������� �Ǵ�С����) ���ܰ���[]��ʽ����Ԫ�� 
// �����  

//setԪ�ص����/����/ɾ����������

void main901()
{
	set<int> set1;
	for (int i=0; i<5; i++)
	{
		int tmp = rand();
		set1.insert(tmp);
	}
	set1.insert(100);
	set1.insert(100);
	set1.insert(100);
	cout << set1.size() << endl;     //ֻ�ɹ�����һ��100

	//��ӡ���
	for (set<int>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< *it << " ";
	}
	cout << endl;

	//ɾ������
	cout << "\nɾ������";
	while (!set1.empty())
	{
		set<int>::iterator it = set1.begin();
		printf("%d ", *it);
		set1.erase(it);
	}
}

//2 ���� ��С���� �Ӵ�С
void main902()
{
	set<int, greater<int>> set1;
	for (int i=0; i<5; i++)
	{
		int tmp = rand();
		set1.insert(tmp);
	}
	set1.insert(100);

	//��ӡ���
	for (set<int, greater<int>>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< *it << " ";
	}
	cout << endl;

	//ɾ������
	cout << "\nɾ������";
	while (!set1.empty())
	{
		set<int, greater<int>>::iterator it = set1.begin();
		printf("%d ", *it);
		set1.erase(set1.begin());
	}
}

//3 �Զ����������� ����
//03 �º��� �������� ����() ���� ���бȽϴ�С
//��Ŀ��ѧ������ѧ�ţ��������ԣ���Ҫ��������뼸��ѧ������set�����У�
//ʹ�������е�ѧ����ѧ�ŵ���������
class Student
{
public:
	Student(char *name, int age)
	{
		strcpy(this->name, name);
		this->age = age;
	}
protected:
public:
	char name[64];
	int age ;
};

//�������� ��ѧ����������
struct StuFunctor
{
	bool operator()(const Student &left, const Student &right)
	{
		return (left.age < right.age); 
	}
};

int main903()
{
	set<Student, StuFunctor> set1;
	Student s1("s1", 31);
	Student s2("s2", 22);
	Student s3("s3", 44);
	Student s4("s4", 11);
	Student s5("s5", 31);
	set1.insert(s1);
	set1.insert(s2);
	set1.insert(s3);
	set1.insert(s4);
	set1.insert(s5); //�������31�� �ܲ���ɹ�  

	set1.insert(Student("��1", 32));
	set1.insert(Student("��2", 32) );   //�ظ� ����ԭ�ȵ�
	set1.insert(Student("��3", 53) );
	set1.insert(Student("��4", 34) );

	//��ӡ���
	for (set<Student, StuFunctor >::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< (*it).name << " ";    //1(32) 4(34) 3(53)
	}
	return 0;
}

//typedef pair<iterator, bool> _Pairib;
//4 ����ж� set1.insert�����ķ���ֵ
//Pair���÷� 
void main904()
{
	Student s1("s1", 31);
	Student s2("s2", 22);
	Student s3("s3", 44);
	Student s4("s4", 11);
	Student s5("s5", 31);

	set<Student, StuFunctor> set1;
	pair<set<Student, StuFunctor>::iterator, bool> pair1 = set1.insert(s1);
	if (pair1.second == true)
	{
		cout << "����s1�ɹ�" << endl;
	}
	else
	{
		cout << "����s1ʧ��" << endl;
	}

	set1.insert(s2);

	//���֪�� ���� �Ľ��
	pair<set<Student, StuFunctor>::iterator, bool> pair5 = set1.insert(s5); //�������31�� �ܲ���ɹ�  
	if (pair5.second == true)
	{
		cout << "����s1�ɹ�" << endl;
	}
	else
	{
		cout << "����s1ʧ��" << endl;
	}

	//����
	for (set<Student, StuFunctor>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << it->age << "\t" << it->name << endl;
	}
}

//05��ʾ��set���ϲ��ҹ���
int main05()
{
	int		i = 0;
	set<int> set1;

	for (i=1; i<10; i++)
	{
		set1.insert(i);
	}

	set<int>::iterator it1 =  set1.lower_bound(5); //С�ڵ���5������
	set<int>::iterator it2 =  set1.upper_bound(5); //���ڵ���5�ĵ�����

	//ͨ������������Ԫ�صĲ���
	cout<<"it1 "<<*it1<<" "<<"it2 "<<*it2<<endl;

	//������һ�Ե�����i��j������i���ڲ��ƻ������ǰ���£�value�ɲ���ĵ�һ��λ�ã��༴lower_bound����
	//j�����ڲ��ƻ������ǰ���£�value�ɲ�������һ��λ�ã��༴upper_bound����
	//��ˣ�[i,j)�ڵ�ÿ��Ԫ�ض���ͬ��value������[i,j)��[first,last)֮�з��ϴ�һ���ʵ����������
	pair <set<int>::iterator, set<int>::iterator> pairIt = set1.equal_range(5);

	set<int>::iterator it3 = pairIt.first; //��ȡ��һ��
	set<int>::iterator it4 = pairIt.second; //��ȡ�ڶ���

	cout<<"it3 "<<*it3<<" "<<"it4 "<<*it4<<endl;
	return 0;
}



