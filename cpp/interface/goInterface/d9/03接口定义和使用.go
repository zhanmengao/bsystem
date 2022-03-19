package main

import "fmt"

//先定义接口  在根据接口实现功能
type Humaner interface {
	//方法  方法声明
	SayHello()
	//Result(int, int) int
}

type Student struct {
	name  string
	age   int
	sex   string
	score int
}

type Teacher struct {
	name    string
	age     int
	sex     string
	subject string
}

func (s *Student) SayHello() {
	fmt.Printf("大家好，我是%s，我今年%d岁，我是%s生，我的成绩是%d分\n",
		s.name, s.age, s.sex, s.score)
}

func (t *Teacher) SayHello() {
	fmt.Printf("大家好，我是%s，我今年%d岁，我是%s生，我的学科是%s\n",
		t.name, t.age, t.sex, t.subject)
}

func main() {
	//接口是一种数据类型 可以接收满足对象的信息
	//接口是虚的  方法的实的
	//接口定义规则  方法实现规则
	//接口定义的规则 在方法中必须有对应的实现
	var h Humaner

	//对象创建
	stu := Student{"小明", 18, "男", 99}
	//stu.SayHello()
	//将对象信息赋值给接口类型变量
	h = &stu
	h.SayHello()

	tea := Teacher{"法师", 31, "男", "go语言与区块链"}
	//tea.SayHello()
	//将对象赋值给接口 必须满足接口中方法的声明格式
	h = &tea
	h.SayHello()

}
