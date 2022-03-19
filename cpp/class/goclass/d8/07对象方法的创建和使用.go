package main

import "fmt"

type student5 struct {
	name  string
	age   int
	sex   string
	score int
	addr  string
}

//函数
func Print() {
	fmt.Println("hello world")
}

//结构体类型可以作为对象类型
func (s student5) Print() {
	fmt.Printf("%p\n",&s)

	fmt.Printf("姓名：%s\n", s.name)
	fmt.Printf("年龄：%d\n", s.age)
	fmt.Printf("性别：%s\n", s.sex)
	fmt.Printf("成绩：%d\n", s.score)
	fmt.Printf("地址：%s\n", s.addr)
}

func main() {

	//创建对象
	stu := student5{"贾政", 58, "男", 80, "贾府"}
	fmt.Printf("%p\n",&stu)
	//对象.方法
	stu .Print()
	fmt.Println(stu)

	stu1 := student5{"贾玲", 29, "女", 99, "北京"}
	stu1.Print()

	//打印错误日志使用函数
	//print()
	//函数调用
	//Print()

	//对象的方法名可以和函数名重名  但是相同的对象方法名不能重名
	//fmt.Println(stu.Print)
	//fmt.Println(stu1.Print)
	//打印函数在代码区的地址
	fmt.Println(Print)
}
