package main

import "fmt"

func main1101() {

	a := 10
	b := 20

	//c := a > b //flase

	//逻辑非 !  非真为假 非假为真
	fmt.Println(!(a > b))
	//单目运算符
	// ! ++ -- &(取地址运算符)
	//双目运算符

}

func main1102() {

	a := 10
	b := 20

	//逻辑与  &&  同真为真 其余为假

	c := a < b && false
	fmt.Println(c)

}

func main1103() {
	a := 10
	b := 20
	//逻辑或  ||  同假为假 其余为真
	fmt.Println(a < b || a > b)
}

func main() {
	a := 10
	b := 20

	//逻辑与高于逻辑或
	fmt.Println(a > b && b > a || a > 0)
}
