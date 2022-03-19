package main

import "fmt"

func test1(a int, b int) {

	fmt.Println(a + b)
}

//函数参数传递时如果有多个参数 中间用逗号分隔
//在函数调用时 使用的是定义中形参的值
func test(n1, n2 int) {
	test1(n1, n2)

}

func main0401() {
	a := 10
	b := 20
	test(a, b)
}

func test3(arr ...int) {

	fmt.Println(arr)
}

//如果不定参函数调用时传递的参数为多个 不定参要写在其他参数后面
func test2(arr ...int) {
	//不能将不定参的名称传递给另外一个不定参
	//test3(arr[0], arr[1], arr[2], arr[3])
	//传递指定个数的数据
	test3(arr[0:4]...)
}

//不定参函数的嵌套调用
func main() {
	test2(1, 2, 3, 4)
}
