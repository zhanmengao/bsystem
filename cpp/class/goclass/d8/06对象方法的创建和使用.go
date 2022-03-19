package main

import "fmt"


//1、定义函数类型  定义结构体名称
//2、为已存在的数据类型起别名
type Int int

//对象的方法

//func (对象)方法(参数列表)(返回值列表){
//	代码体
//}

func (a Int) add(b Int) Int {

	fmt.Println(a)
	fmt.Println(b)
	return a + b
}
func main() {

	//将源文件编译成可执行程序
	//编译过程
	//1、预处理 包展开 替换数据类型  去掉注释
	//2、编译  如果代码有错会提示 如果没错会编译成汇编文件
	//3、汇编  将汇编文件转成二进制文件
	//4、链接  将支持的库链接到程序中 变成可执行程序

	//类型别名会在编译时进行转换
	var a Int = 10

	var b Int =20
	//var b Int = 20
	//包.函数 结构体.成员 对象.方法
	//sum:=a.add(b)

	//对象.方法
	//想要使用方法 必须是相同类型的对象才可以
	sum:=b.add(a)
	fmt.Println(sum)
	//var b int = 20

	//fmt.Println(a + 10)

	//fmt.Printf("%T\n", a)

}
