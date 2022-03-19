package main

import (
	"fmt"
	"math"
)

func main0201() {
	//变量的定义和使用
	//var 变量名 数据类型 = 值
	//int 表示整型数据
	var sun int = 50

	//变量在程序运行过程中 值可以发生改变
	//表达式
	sun = sun + 25

	fmt.Println(sun)
}
func main0202() {

	//变量的声明 如果没有赋值 默认值为0
	var sun int

	//为变量赋值
	sun = 50

	fmt.Println(sun)
}

func main0203() {
	//float64 浮点型数据
	var value float64 = 2

	//var sum int = value * value * value * value * value * value * value * value * value * value

	//可以使用系统提供的包 计算数据的n次方
	//需要导入math包 pow函数
	var sum float64 = math.Pow(value, 10)

	fmt.Println(sum)
}
