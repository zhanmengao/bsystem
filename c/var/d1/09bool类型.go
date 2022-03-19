package main

import "fmt"

func main() {
	//布尔类型  值为true 或者为 false
	//var a bool //默认值为false
	////bool类型一般用于条件判断
	//a = true
	//fmt.Println(a)

	//自动推到类型创建bool类型变量
	a := false //bool

	//a=true
	//fmt.Println(a)
	//%T是一个占位符 表示输出一个变量对应的数据类型
	fmt.Printf("%T\n", a)
	b := 10 //int
	fmt.Printf("%T\n", b)
	c := 3.14 //float64
	fmt.Printf("%T\n", c)
	d := "你个锤子"//string
	fmt.Printf("%T", d)
}
