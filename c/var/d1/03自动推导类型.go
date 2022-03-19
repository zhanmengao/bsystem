package main

import "fmt"

func main0301() {
	//var a int = 10
	//var b int = 20
	//不同的数据类型在内存中开辟的空间不同
	//自动推导类型
	a := 10      //int     整型
	b := 123.456 //float64 浮点型
	c := "传智播客"  //string 字符串类型

	fmt.Println(a + 10)
	fmt.Println(b + 3.14)
	fmt.Println(c)
	//不同的数据类型不能计算
	//fmt.Println(a + b)//err
}

//交换两个变量的值
func main() {
	a := 10
	b := 20

	//使用第三变量进行交换
	//先定义后使用
	//var c int
	//c = a

	//c := a
	//a = b
	//b = c

	//通过运算进行交换
	a = a + b
	b = a - b
	a = a - b

	fmt.Println(a)
	fmt.Println(b)
}
