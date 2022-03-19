package main

import "fmt"

type FuncType func(int, int)

func main0801() {

	a := 10
	b := 20

	//在函数内部定义一个匿名函数
	//f 函数类型变量 接收函数

	var f FuncType

	f = func(a int, b int) {
		fmt.Println(a + b)
	}

	f(a, b)
	//fmt.Println(f)

}
func main0802() {
	//a := 10
	//b := 20
	// int类型
	//v := func(a int, b int) int {
	//	return a + b
	//}(a, b)

	//如果定义匿名函数后在{}后面如果有()表示函数调用
	f := func(a int, b int) int {
		return a + b
	}

	v := f(10, 20)
	fmt.Printf("%T\n", f)
	fmt.Println(v)
}
func main0803() {
	a := 10
	b := 20

	f := func() int {
		return a + b
	}

	//v := f(a, b)

	a = 100
	b = 200

	v := f()

	fmt.Println(v)
}
