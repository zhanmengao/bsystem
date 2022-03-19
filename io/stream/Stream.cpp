#include<iostream>
using namespace std;
void mainStream()
{
	//good:流未处于错误 bad:流已崩溃 fail:一个Io操作失败 eof:读到了文件结束
	cout <<"cin state:"<<cin.good() << cin.bad() << cin.eof() << cin.fail() << endl;
	auto old_state = cin.rdstate();			 //记住cin的当前状态
	cin.clear();										 //使cin有效
	cin.setstate(old_state);						//将cin置位

	//标准库将cin和cout关联，所以任何从cin读取数据的操作都会刷新cout。
	cin.tie(nullptr);                          //取消关联
	cin.tie(&cout);                          //重新关联
	//除此之外：
	cout << "hi!" << endl;				 //endl输出换行并且刷新缓冲区
	cout << "hi!" << flush;				 //刷新缓冲区，无任何附加字符
	cout << "hi!" << ends;			 //ends输出空格并刷新缓冲区
	cout << unitbuf;						 //设置cout无缓冲区
	cout << nounitbuf;					 //回到正常的缓冲方式


}