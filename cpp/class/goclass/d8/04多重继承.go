package main

import "fmt"

type person2 struct {
	name string
	age  int
	sex  string
}
type person3 struct {
	id   int
	addr string
}

type student2 struct {
	//结构体成员为多个匿名字段
	person2
	person3

	class int
	score int
}

func main() {
	//var stu student2
	//
	//stu.person3.id = 1001
	//stu.person2.name = "亚索"
	//stu.age = 10
	//stu.score = -5
	//stu.class = 300
	//stu.addr = "召唤师峡谷"
	//stu.sex = "男"

	stu:=student2{person2{"劫",12,"男"},person3{1002,"召唤师峡谷"},300,-5}

	fmt.Println(stu)

}
