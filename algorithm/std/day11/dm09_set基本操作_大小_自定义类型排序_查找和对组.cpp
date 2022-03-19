#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "set"
#include <xfunctional>
//1 集合 元素唯一 自动排序(默认情况下 是从小到大) 不能按照[]方式插入元素 
// 红黑树  

//set元素的添加/遍历/删除基本操作

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
	cout << set1.size() << endl;     //只成功插入一个100

	//打印输出
	for (set<int>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< *it << " ";
	}
	cout << endl;

	//删除集合
	cout << "\n删除集合";
	while (!set1.empty())
	{
		set<int>::iterator it = set1.begin();
		printf("%d ", *it);
		set1.erase(it);
	}
}

//2 集合 从小到大 从大到小
void main902()
{
	set<int, greater<int>> set1;
	for (int i=0; i<5; i++)
	{
		int tmp = rand();
		set1.insert(tmp);
	}
	set1.insert(100);

	//打印输出
	for (set<int, greater<int>>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< *it << " ";
	}
	cout << endl;

	//删除集合
	cout << "\n删除集合";
	while (!set1.empty())
	{
		set<int, greater<int>>::iterator it = set1.begin();
		printf("%d ", *it);
		set1.erase(set1.begin());
	}
}

//3 自定义数据类型 排序
//03 仿函数 函数对象 重载() 操作 进行比较大小
//题目：学生包含学号，姓名属性，现要求任意插入几个学生对象到set容器中，
//使得容器中的学生按学号的升序排序
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

//函数对象 按学生年龄排序
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
	set1.insert(s5); //如果两个31岁 能插入成功  

	set1.insert(Student("张1", 32));
	set1.insert(Student("张2", 32) );   //重复 保留原先的
	set1.insert(Student("张3", 53) );
	set1.insert(Student("张4", 34) );

	//打印输出
	for (set<Student, StuFunctor >::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout<< (*it).name << " ";    //1(32) 4(34) 3(53)
	}
	return 0;
}

//typedef pair<iterator, bool> _Pairib;
//4 如何判断 set1.insert函数的返回值
//Pair的用法 
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
		cout << "插入s1成功" << endl;
	}
	else
	{
		cout << "插入s1失败" << endl;
	}

	set1.insert(s2);

	//如何知道 插入 的结果
	pair<set<Student, StuFunctor>::iterator, bool> pair5 = set1.insert(s5); //如果两个31岁 能插入成功  
	if (pair5.second == true)
	{
		cout << "插入s1成功" << endl;
	}
	else
	{
		cout << "插入s1失败" << endl;
	}

	//遍历
	for (set<Student, StuFunctor>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << it->age << "\t" << it->name << endl;
	}
}

//05演示：set集合查找功能
int main05()
{
	int		i = 0;
	set<int> set1;

	for (i=1; i<10; i++)
	{
		set1.insert(i);
	}

	set<int>::iterator it1 =  set1.lower_bound(5); //小于等于5迭代器
	set<int>::iterator it2 =  set1.upper_bound(5); //大于等于5的迭代器

	//通过迭代器进行元素的操作
	cout<<"it1 "<<*it1<<" "<<"it2 "<<*it2<<endl;

	//它返回一对迭代器i和j，其中i是在不破坏次序的前提下，value可插入的第一个位置（亦即lower_bound），
	//j则是在不破坏次序的前提下，value可插入的最后一个位置（亦即upper_bound），
	//因此，[i,j)内的每个元素都等同于value，而且[i,j)是[first,last)之中符合此一性质的最大子区间
	pair <set<int>::iterator, set<int>::iterator> pairIt = set1.equal_range(5);

	set<int>::iterator it3 = pairIt.first; //获取第一个
	set<int>::iterator it4 = pairIt.second; //获取第二个

	cout<<"it3 "<<*it3<<" "<<"it4 "<<*it4<<endl;
	return 0;
}



