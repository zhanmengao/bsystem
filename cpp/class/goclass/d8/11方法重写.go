package main

import "fmt"

type person8 struct {
	id   int
	name string
	age  int
	sex  string
}

type student8 struct {
	person8

	class int
	score int
	addr  string
}

func (p *person8) SayHello() {
	fmt.Printf("大家好我是：%s，我今年%d岁，我是%s生\n", p.name, p.age, p.sex)
}

//子类对象和父类对象的方法名重名
func (s *student8) SayHello() {
	fmt.Printf("大家好我是：%s，我今年%d岁，我是%s生，我的班级是%d，我的成绩是%d\n",
		s.name, s.age, s.sex, s.class, s.score)

}
func main() {

	stu := student8{person8{1001, "甘夫人", 32, "女"}, 112, 80, "巴蜀"}

	//子类对象方法  采用就进原则 使用子类对象方法
	//如果子类和父类有相同的方法名 叫方法重写
	stu.SayHello()
	//父类对象方法
	stu.person8.SayHello()
	fmt.Println(stu.SayHello)
	fmt.Println(stu.person8.SayHello)
}
