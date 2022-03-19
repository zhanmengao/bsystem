package main

import "fmt"

func main0501() {
	//var a int = 10
	//常量的定义  一般定义常量使用大写字母
	const MAX int = 10
	//常量可以参与程序的计算
	//b:=a+10
	//常量不允许左值赋值
	//a=a+10//err
	//a=100//err
	fmt.Println(MAX)

}
func main0502() {
	const MAX = "你瞅啥"

	//fmt.Println(MAX)
	//fmt.Printf("%T\n",MAX )
	//go语言常量的地址 不允许访问
	//fmt.Printf("%p",&MAX)//err
}

func main0503() {
	//计算圆的面积和周长
	//面积 PI*r*r  math.Pow(r,2)
	//周长 2*PI*r

	//PI 3.14
	//常量必须定义
	const PI float64 = 3.14
	var r float64

	fmt.Scan(&r)
	//面积
	s := PI * r * r
	//周长
	l := 2 * PI * r
	//fmt.Println(PI)
	fmt.Printf("面积:%.2f\n", s)
	fmt.Printf("周长:%.2f\n", l)

}

func main() {
	//数据区 -》常量区
	//字面常量
	fmt.Println("hello world")
	fmt.Println(3.14)
	a := 10
	fmt.Println(a + 20)

}
