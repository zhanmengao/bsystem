#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "string"
#include "algorithm"

//string的初始化
void main2015()
{
	string s1 = "aaaa";
	string s2("bbbb");
	string s3 = s2; //通过拷贝构造函数 来初始化对象s3
	string s4(10, 'a');

	cout << "s1:" << s1 << endl;
	cout << "s2:" << s2 << endl;
	cout << "s3:" << s3 << endl;
	cout << "s4:" << s4 << endl;
}

//string的 遍历
void main202()
{
	string s1 = "abcdefg";

	//1 数组方式
	for (int i=0; i<s1.length(); i++)
	{
		cout << s1[i] << " ";
	}
	cout << endl;

	//2 迭代器
	for (string::iterator it = s1.begin(); it != s1.end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl;

	//3 at函数 和 operator[] 的区别  -> 是否允许Catch
	try
	{
		for (int i=0; i<s1.length() + 3; i++)
		{
			cout << s1.at(i) << " ";  //抛出异常
		}
	}
	catch ( ... )
	{
		cout << "发生异常\n" ;
	}

	cout << "at之后" << endl;
	
	//try
	//{
	//	for (int i=0; i<s1.length() + 3; i++)
	//	{
	//		cout << s1[i] << " "; //出现错误 不向外面抛出异常，直接引起程序的中断 -》 因为operator[]函数不会对下标做检查
	//	}
	//}
	//catch ( ... )
	//{
	//	cout << "发生异常\n" ;
	//}
	
	
}

//字符指针和string的转换
void main2013()
{
	string s1 = "aaabbbb";

	printf("s1:%s \n", s1.c_str());        //1 s1===>char *

	string s2("aaabbb");               	//2 char *====>sting 

	//3 s1的内容 copy buf中
	char buf1[128] = {0};
	s1.copy(buf1, 3, 0);  //将0开始的3个字符 拷贝到buf1所指向的内存。注意 只给你copy3个字符 不会变成C风格的字符串
	cout << "buf1:" << buf1 << endl; 
}

//字符串的 连接
void main204()
{
	string s1 = "aaa";
	string s2 = "bbb";
	s1 = s1 + s2;           //拼接法1 operator+
	cout << "s1:" << s1 << endl;


	string s3 = "333";
	string s4 = "444";
	s3.append(s4);    	    //拼接法2 append函数
	cout << "s3:" << s3 << endl;
}


//字符串的查找和替换
void main205()
{
	string s1 = "wbm hello wbm 111  wbm 222  wbm 333 ";
	//			 ▲
	//第一次 出现wbm index

	int index = s1.find("wbm", 0); //位置下标 从0开始
	cout << "index: " << index << endl;

	//案例1 求wbm出现的次数 每一次出现的数组下标
	int offindex = s1.find("wbm", 0);
	while (offindex != string::npos)
	{
		cout << "offindex:" << offindex << endl;
		offindex = offindex + 1;
		offindex = s1.find("wbm", offindex); 
	}

	//案例2  把小写wbm===>WBM  replace(pos,size,src)

	string s3 = "aaa  bbb ccc";
	s3.replace(0, 3, "AAA");
	cout << "s3" << s3 << endl;

	offindex = s1.find("wbm", 0);
	while (offindex != string::npos)
	{
		cout << "offindex:" << offindex << endl;
		s1.replace(offindex,3, "WBM");                      //将找到的wbm的index开始数3个字节 替换为 WBM
		offindex = offindex + 1;
		offindex = s1.find("wbm", offindex); 
	}

	//案例3 assign
	s3.assign(s1);
	cout << "s3替换后的结果: " << s3 << endl;

	s3.assign(3, 'aaa');
	cout << "s3替换后的结果: " << s3 << endl;
}


//erase 截断（区间删除）   和   insert 插入
void main206()
{
	//区间删除
	string s1 = "hello1 hello2 hello1";
	string::iterator it = find(s1.begin(), s1.end(), 'l');
	if (it != s1.end() )
	{
		s1.erase(it);
	}
	cout << "s1删除l以后的结果:" << s1 << endl;

	s1.erase(s1.begin(), s1.end() );
	cout << "s1全部删除:" << s1 << endl;
	cout << "s1长度 " << s1.length() << endl;

	//区间插入
	string s2 = "BBB";
	s2.insert(0, "AAA");             // 头插法
	s2.insert(s2.length(), "CCC");   //  尾插法
	cout << s2 << endl;
}

//对字符串批处理
void main207()
{
	string s1 = "AAAbbb";
	transform(s1.begin(), s1.end(),s1.begin(), toupper);      //1函数的入口地址 2函数对象 3函数指 ---->>> 将[begin,end)区间内的元素，经过函数处理后放到to
	cout << "s1" << s1 << endl;

	string s2 = "AAAbbb";
	transform(s2.begin(), s2.end(), s2.begin(), tolower);
	cout << "s2:" << s2 << endl;

}

//比较
void main208()
{
	string s1 = "aaaaaa";
	string s2 = "aaaaab";
	const char* c3 = "aaaaab";

	int ret = s1.compare(s2);
	cout << "s1 = " << s1 << " s2 = " << s2 <<"    ret = "<< ret<<endl;

	ret = s1.compare(2,s1.size()-2,s2);    //从2开始的s1.size()-2个字符
	cout << "s1 = " << s1 << " s2 = " << s2 << "    ret = " << ret << endl;

	ret = s1.compare(2, s1.size() - 2, s2,2,s2.size()-2);    //从2开始的s1.size()-2个字符 VS 从2开始的s2 size-2个字符
	cout << "s1 = " << s1 << " s2 = " << s2 << "    ret = " << ret << endl;

	ret = s1.compare(c3);
	cout << "s1 = " << s1 << " c3 = " << string(c3) << "    ret = " << ret << endl;
}

//子串 substr
void main209()
{
	string s1 = "hello world!";

	string sub1 = s1.substr(0, 5);
	cout << "sub1 = " << sub1 << endl;

	string sub2 = s1.substr(1, 2);
	cout << "sub2 = " << sub2 << endl;
}
