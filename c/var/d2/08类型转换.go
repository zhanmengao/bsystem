package main

import "fmt"

func main0801() {

	a, b, c := 0, 0, 0

	fmt.Scan(&a, &b, &c)

	sum := a + b + c
	fmt.Println(sum)
	//类型转换 数据类型(变量)  数据类型(表达式)
	//fmt.Println(float64(sum / 3))
	fmt.Printf("%.2f", float64(sum)/3)
}

func main0802() {
	//var a float32 = 12.34
	//var b float64 = 22.22

	//int8 -> int16 ->int32 ->int64
	//float32 ->float64

	//int64 -> float64
	//在类型转换时建议低类型转成高类型  保证数据精度
	//建议整型转成浮点型
	//高类型转成低类型 可能会丢失精度
	//fmt.Printf("%.2f", float64(a)+b)

	//fmt.Println(float64(a) + b)

	//数据溢出 符号发生变化
	//var a int = 1234
	//fmt.Println(int8(a))
	var a float64 = 3.999

	//将浮点型转成整型 保留数据整数部分 丢弃小数部分
	b := int(a) //3
	fmt.Println(b)
}

//练习题
func main() {
	//107653几天几小时几分几秒

	s := 107653

	fmt.Println("天：", s/60/60/24%365)
	fmt.Println("时：", s/60/60%24)
	fmt.Println("分：", s/60%60)
	fmt.Println("秒：", s%60)

	//1*24*60*60 + 5*60*60 + 54*60 + 13 =107653
}
