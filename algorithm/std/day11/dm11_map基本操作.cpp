
#include <iostream>
using namespace std;
#include "map"
#include "string"


//map元素的添加/遍历/删除基本操作
void main1101()
{
	map<int, string> map1;

	//方法1
	map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );
	
	

	//方法2 
	map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );

	//方法3 
	map1.insert(map<int, string>::value_type(5, "teacher05") );
	map1.insert(map<int, string>::value_type(6, "teacher06") );

	//方法4
	map1[7] = "teacher07";
	map1[8] = "teacher08";

	//在末尾添加
	if (map1.find(3) == map1.end())
	{
		map1[3] = "aaa";
	}
	
	//map1['z'] = "teacher08";

	//容器的遍历
	for (map<int, string>::iterator it = map1.begin(); it!=map1.end(); it++ )
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "遍历结束" << endl;


	//容器元素的删除
	while (!map1.empty())
	{
		map<int, string>::iterator it = map1.begin();
		cout << it->first << "\t" << it->second << endl;
		map1.erase(it);
	}
	for (map<int, string>::iterator it = map1.begin(); it != map1.end();)
	{
		cout << it->first << "\t" << it->second << endl;
		it = map1.erase(it);
	}
}

//插入的四种方法 异同
//前三种方法 返回值为pair<iterator,bool>	若key已经存在 则报错
//	方法四									若key已经存在,则修改									
void main1102()
{
	map<int, string> map1;

	//检查返回值

	//方法1
	pair<map<int, string>::iterator, bool>  mypair1 =  map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );

	//方法2 
	pair<map<int, string>::iterator, bool>  mypair3 = map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );

	//方法3 
	pair<map<int, string>::iterator, bool>  mypair5 = map1.insert(map<int, string>::value_type(5, "teacher05") );
	if (mypair5.second != true)
	{
		cout << "key 5 插入失败" << endl;
	}
	else
	{
		cout << mypair5.first->first << "\t" <<  mypair5.first->second <<endl;
	}

	
	pair<map<int, string>::iterator, bool>  mypair6 =  map1.insert(map<int, string>::value_type(5, "teacher55") );
	if (mypair6.second != true)
	{
		cout << "key 6 插入失败" << endl;
	}
	else
	{
		cout << mypair6.first->first << "\t" <<  mypair6.first->second <<endl;
	}

	//方法4
	map1[7] = "teacher07";
	map1[7] = "teacher77";

	//容器的遍历
	for (map<int, string>::iterator it = map1.begin(); it!=map1.end(); it++ )
	{
		cout << it->first << "\t" << it->second << endl;
	}
	cout << "遍历结束" << endl;

}

void main1103()
{
	map<int, string> map1;
	map1.insert(pair<int, string>(1,"teacher01") );
	map1.insert(pair<int, string>(2,"teacher02") );
	map1.insert(make_pair(3, "teacher04") );
	map1.insert(make_pair(4, "teacher05") );
	map1.insert(map<int, string>::value_type(5, "teacher05") );
	map1.insert(map<int, string>::value_type(6, "teacher06") );
	map1[7] = "teacher07";
	map1[8] = "teacher08";

	//map的查找 //异常处理
	map<int, string>::iterator it2 = map1.find(100);
	if (it2 == map1.end())
	{
		cout << "key 100 的值 不存在" << endl;
	}
	else
	{
		cout << it2->first << "\t" << it2->second << endl;
	}

	//equal_range //异常处理
	pair<map<int, string>::iterator , map<int, string>::iterator> mypair = map1.equal_range(5); //返回两个迭代器 形成一个 pair
	//第一个迭代器 >= 5的 位置 
	//第一个迭代器 = 5的 位置 
	if (mypair.first == map1.end() )
	{
		cout << "第一个迭代器 >= 5的 位置 不存在" << endl;
	}
	else
	{
		cout << mypair.first->first << "\t" << mypair.first->second << endl;
	}
	//使用第二个迭代器
	if (mypair.second == map1.end() )
	{
		cout << "第二个迭代器 > 5的 位置 不存在" << endl;
	}
	else
	{
		cout << mypair.second->first << "\t" << mypair.second->second << endl;
	}
}