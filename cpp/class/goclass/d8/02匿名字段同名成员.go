package main

import "fmt"

type person struct {
	id   int
	name string
	age  int
	sex  string
}

type student struct {
	person

	name  string
	class int
	score int
}

func main() {

	//对象初始化
	//var stu student
	//stu.score = 100
	//stu.class = 301
	//stu.id = 1002
	//stu.age = 18
	//stu.sex = "男"
	////子类赋值
	////子类和父类结构体有相同的成员名 默认赋值为子类 采用就近原则
	//stu.name = "瞎子"
	////父类赋值
	//stu.person.name = "李青"
	stu:=student{person{1002,"李青",18,"男"},"李青",301,100}

	fmt.Println(stu)
}
