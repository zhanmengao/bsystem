package main

import "fmt"

type Student struct {
	id    int
	name  string
	age   int
	sex   string
	score int
	addr  string
}

//结构体变量作为函数参数
func test(stu Student) {
	stu.name = "野猪佩奇"
	fmt.Println(stu)

}

func main0201() {
	stu := Student{101, "喜羊羊", 6, "男", 100, "羊村"}
	//值传递
	test(stu)

	fmt.Println(stu)
}
func test1(m map[int]student) {
	//指针不能直接.成员
	//m[102].name = "威震天"//err

	stu := m[102]
	stu.name = "威震天"
	m[102] = stu
	//fmt.Println(stu)
	//fmt.Printf("%T\n", stu)
	//fmt.Println(m[102])
}
func main() {

	//将结构体作为map中的value
	m := make(map[int]student)

	//map中的数据不建议排序操作
	m[101] = student{"擎天柱", "男", 30, 100, "赛博坦星球"}
	m[102] = student{"大黄蜂", "男", 10, 59, "赛博坦星球"}

	//将map作为函数参数
	test1(m)
	fmt.Println(m)

}
