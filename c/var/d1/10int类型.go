package main

import "fmt"

func main1001() {

	//int类型会根据操作系统位数不同在内存中占的字节也不同  int64 int32
	var a int = 0

	b := 100000000
	fmt.Println(a)
	fmt.Println(b)
}

func main1002() {
	//无符号整型数据 存储的0 ~ 2^64-1
	var a uint = 10
	//如果出现了负数  会溢出从最小值变成最大值
	a = a - 20
	fmt.Println(a)

}

//func main() {
//	var a int = 10
//	var b int64 = 20
//	//定义int和int64 需要使用类型转换才可以计算
//	fmt.Println(a + b)
//
//	float64  + float32
//}
