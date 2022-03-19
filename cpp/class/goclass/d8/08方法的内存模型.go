package main

import "fmt"

type student6 struct {
	name  string
	age   int
	sex   string
	score int
	addr  string
}

//对象不同方法名相同 不会冲突
//在方法调用中 方法的接收者为指针类型
//指针类型 和普通类型表示是相同对象的类型

func (s *student6) Print() {
	s.name = "黄月英"
	s.score = 99
	//fmt.Println(*s)

}
func main() {
	stu := student6{"貂蝉", 22, "女", 80, "徐州"}

	//地址传递(*student6) 值传递(student6)
	//一般建议选择地址传递
	stu.Print()

	fmt.Println(stu)
}
