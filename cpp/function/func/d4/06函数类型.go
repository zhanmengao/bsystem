package main

import "fmt"

func demo1(a int, b int) {
	fmt.Println(a + b)
}

//func (int,int)int函数类型
func demo2(s1 int, s2 int) {
	fmt.Println(s1 + s2)
}

//定义函数类型 为已存在的数据类型起别名
type FUNCDEMO func(int, int)

func main() {
	//demo1(10, 20)
	//函数的名字表示一个地址 函数在代码区的地址
	fmt.Println(demo1)

	//f是func (int,int)函数类型定义的变量
	//var f func(int, int)
	var f FUNCDEMO
	//函数类型定义的变量结束函数的地址
	f = demo1
	fmt.Println(f)
	//通过f调用函数
	f(10, 20)

	f = demo2
	f(1, 2)

	fmt.Printf("%T", f)
}
