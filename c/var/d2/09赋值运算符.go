package main

import "fmt"

func main() {
	//a := 10
	//b := 20
	//c := a + b
	//
	////c += 20 //c=c+20
	////c -= 20
	////c *= 20
	////c /= 20 //30
	//c = 20
	//c %= 3 //c = c % 3

	var c int = 10
	//将表达式右侧进行结果计算在进行赋值运算符
	c %= (2 + 3)

	//c = c % 5 //ok
	//c = c % 2 + 3//err

	fmt.Println(c)
}
