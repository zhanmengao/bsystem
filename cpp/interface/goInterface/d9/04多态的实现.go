package main

import "fmt"

//先定义接口  在根据接口实现功能
type Humaner1 interface {
	//方法  方法声明
	SayHello()
	//Result(int, int) int
}

type Student1 struct {
	name  string
	age   int
	sex   string
	score int
}

type Teacher1 struct {
	name    string
	age     int
	sex     string
	subject string
}

func (s *Student1) SayHello() {
	fmt.Printf("大家好，我是%s，我今年%d岁，我是%s生，我的成绩是%d分\n",
		s.name, s.age, s.sex, s.score)
}

func (t *Teacher1) SayHello() {
	fmt.Printf("大家好，我是%s，我今年%d岁，我是%s生，我的学科是%s\n",
		t.name, t.age, t.sex, t.subject)
}

//多态的实现
//将接口作为函数参数  实现多态
func SayHi(h Humaner1){
	h.SayHello()
}

func main() {

	stu:=Student1{"小明",18,"男",99}
	//调用多态函数
	SayHi(&stu)

	//tea:=Teacher1{"法师",31,"男","go"}
	//SayHi(&tea)

}

