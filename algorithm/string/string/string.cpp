#include<string>
#include<iostream>
#include<stdio.h>
#include<string.h>

int main(){
	char cs[] = "aaa\0bbb";
	std::string str(cs);
	std::cout<<"size : "<<str.size()<<" str = "<<str<<std::endl;
	printf("sz : %d. len = %d  : %s \n",sizeof(cs),strlen(cs),cs);
	return 0;
}
