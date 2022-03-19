#include<iostream>;
using namespace std;

/*解决深拷贝，浅拷贝问题
*/

class Name {
public:
	/*
	//拷贝构造函数
	Name(Name &obj)
	{
		//深拷贝
		cout << "我是Name的copy构造函数" << endl;

		//用obj来初始化自己
		this->p = (char*)malloc(obj.len + 1);
		len = obj.len;

	}
	*/
	char *p;
	int len;
	Name(const char * myp)
	{
		cout << "我是Name的构造函数" << endl;
		int len = strlen(myp);
		p = (char *)malloc(len + 1);
	}
	~Name()
	{
		cout << "我是Name的析构函数" << endl;
		if (p != NULL)
		{
			//析构
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
	//构造
	Name n1("abc");
	Name n2 = n1;
	//用二级指针储存两个一级指针变量指向（堆区地址）
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