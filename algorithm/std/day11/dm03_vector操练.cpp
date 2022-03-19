
#include <iostream>
using namespace std;
#include "vector"
//工具函数
void printV(vector<int> &v);
typedef struct
{
	unsigned char  bLength;                   //该描述符结构大小
	unsigned char  bDescriptorType;           //接口描述符的类型编号(0x04)
	unsigned char  bInterfaceNumber;          //接口的编号
	unsigned char  bAlternateSetting;         //备用编号，可以理解为接口的配置号。一个接口只能一个配置有效？
	unsigned char  bNumEndpoints;             //该接口使用的端点数，不包括端点0
	unsigned char  bInterfaceClass;           //接口类型
	unsigned char  bInterfaceSubClass;        //接口子类型
	unsigned char  bInterfaceProtocol;        //接口遵循的协议
	unsigned char  iInterface;                //描述该接口的字符串索引值
} USB_INTERFACE_DESCRIPTOR;

//数组元素的 添加和删除
void mainVV3()
{
	vector<int> v1;

	cout << "length:" << v1.size() <<endl;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	cout << "length:" << v1.size() <<endl;

	cout << "头部元素" << v1.front() <<"    尾部元素" <<v1.back()<<endl;


	//修改 头部元素
	//函数返回值当左值 应该返回一个引用
	v1.front() = 11;
	v1.back() = 55;

	while (v1.size() > 0)
	{
		cout <<"尾部元素" << v1.back() ; //获取尾部元素
		v1.pop_back(); //删除尾部元素
	}

	vector<USB_INTERFACE_DESCRIPTOR> interfacevec;
	for (int i = 0; i < 5; i++)
	{
		USB_INTERFACE_DESCRIPTOR info;
		interfacevec.push_back(info);
	}
}

//vector的初始化
void main3012()
{
	vector<int> v1;              //默认构造
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);

	vector<int> v2 = v1;  //拷贝构造

	vector<int> v3(v1.begin(), v1.begin()+2 );     //通过迭代器构造

	vector<int> v4{ 0, 2, 5, 8, 2 };              //通过初值列表构造

	v4 = { 3, 3, 5, 6, 98 };

	vector<int> v5(10, 1);                      //构造一个size=10的容器 其内部的值均为1
}



//vector的遍历 通过数组的方式 
void main3013()
{
	vector<int> v1(10);   //提前把内存准备好，其内部的值均为随机值

	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	printV(v1);

}

//push_back的强化记忆
void main304() 
{
	vector<int> v1(10);   //提前把内存准备好
	v1.push_back(100);    //在尾部插入-> index=10
	v1.push_back(200);    //index = 11
	cout << "size: " << v1.size() << endl;         //12
	printV(v1);
}


//1迭代器 end()的理解 
//   1	3	5
//	 ▲	
//	          ▲
//当 it == v1.end()的时候 说明这个容器已经遍历完毕了...
//end()的位置 应该是 5的后面

//2 迭代器的种类
/*
typedef iterator pointer;                                                        正向迭代器
typedef const_iterator const_pointer;                                            const迭代器
typedef _STD reverse_iterator<iterator> reverse_iterator;                        逆向迭代器
typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;           const逆向迭代器
*/
void main3015()
{
	vector<int> v1(10);   
	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	//正向遍历
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it ++ )
	{
		cout << *it << " ";
	}

	//逆序遍历
	for (vector<int>::reverse_iterator rit = v1.rbegin(); rit!=v1.rend(); rit++ )
	{
		cout << *rit << " ";
	}

}

//vector  删除
void main306()
{
	vector<int> v1(10);   
	for (int i=0; i<10; i++)
	{
		v1[i] = i + 1;
	}

	//区间删除
	v1.erase(v1.begin(), v1.begin()+3);
	printV(v1);

	//根据元素的位置 指定位置删除
	v1.erase(v1.begin()); //在头部删除一个元素
	printV(v1);

	cout << endl;

	//根据元素的值 
	v1[1] = 2;
	v1[3] = 2;
	printV(v1);

	for (vector<int>::iterator it =v1.begin(); it != v1.end();)
	{
		if (*it == 2)
		{
			it = v1.erase(it);  //当 删除迭代器所指向的元素的时候,erase删除函数会让it自动下移动
		}
		else
		{
			it ++;
		}
	}
	printV(v1);

	cout << endl;
	v1.insert(v1.begin(), 100);
	v1.insert(v1.end(), 200);
	printV(v1);
}

//reserve 设置内部数组大小
void main307()
{
	vector<int> v1;
	v1.reserve(50);
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
		cout << "插入" << i << "，此时=" << v1.capacity() << "。v1.size()=" << v1.size() << "  v1地址为" <<
			&v1 << endl;
	}
}