package main

import "fmt"

func main0701() {

	//变量声明
	var a int

	//通过键盘为变量赋值
	//& 是一个运算符  取地址运算符
	fmt.Scan(&a)
	//内存地址 0xc042058080  是一个十六进制整型数据
	//fmt.Println(&a)

	fmt.Println(a)
}
func main0702() {
	//如果多个变量类型相同可以一次声明多个变量
	var a, b int
	//空格或回车 表示一个输入接收结束
	fmt.Scan(&a, &b)

	fmt.Println(a)
	fmt.Println(b)

}

func main0703() {
	var a int
	var b string
	//在接收字符串时要使用空格作为分割
	fmt.Scanf("%d%s", &a, &b)

	fmt.Println(a)
	fmt.Printf("==%s==", b)
}

//通过键盘输入学生三门成绩计算总成绩和平均成绩

func main() {
	var c, m, e int

	fmt.Scan(&c, &m, &e)

	sum := c + m + e
	fmt.Println("总成绩：", sum)
	//两个整型数据相除 得到的结果也是整型
	fmt.Println("平均成绩：", sum/3)
}
