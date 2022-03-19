#include<iostream>
void test(int *p)
{
	int a=1;
	p=&a;
	std::cout<<p<<" "<<*p<< std::endl;
}

int main333(void)
{
    int *p=NULL;
    test(p);
    if(p==NULL)
    std::cout<<"指针p为NULL"<< std::endl;
    return 0;
}
//运行结果为：
//0x22ff44 1
//指针p为NULL



void testPTR(int* p) {
    int a = 12;
    p = &a;

}

void testREFF(int& p) {
    int a = 12;
    p = a;

}
void main313891238()
{
    int a = 10;
    int* b = &a;
    testPTR(b);//改变指针指向，但是没改变指针的所指的内容
	std::cout << a << std::endl;// 10
	std::cout << *b << std::endl;// 10

    a = 10;
    testREFF(a);
	std::cout << a << std::endl;//12
}