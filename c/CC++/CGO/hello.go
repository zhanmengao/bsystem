package main

/*
#include<stdio.h>
#include<stdlib.h>
void hello(){
	printf("hello world! \n");
}
 */
import "C"

func main(){
	C.hello()
}