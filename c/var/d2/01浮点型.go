package main

import "fmt"

func main0101() {

	//浮点型数据 分为 单精度浮点型 float32（小数位数为7位） 双精度浮点型 float64（小数位数为15位）
	//float64 比 float32 更精准
	//var a float64=1223.456
	////保留六位小数数据会更精准
	//fmt.Printf("%f\n",a)
	//var a float32 =3.14
	//
	//fmt.Printf("%.2f",a)

	//通过自动推到类型创建的浮点型变量 默认类型为float64
	a := 123.456
	fmt.Printf("%T\n", a)
}

//买黄瓜 3.2一斤 买3斤

func main() {
	price := 3.2
	var weight float64

	fmt.Scan(&weight)

	sum := price * weight

	//fmt.Println(sum)
	fmt.Printf("%.2f",sum)
}
