#include"test.h"
#include<stdio.h>

void hello(){
	printf("hello , cgo \n");
}

#ifdef _TEST_

int main(){
	hello();
	return 0;
}

#endif
