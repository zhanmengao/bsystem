package main

import "fmt"

//结构体定义在函数外部
//定义函数类型
//type  functye func(int,int)int

//type 结构体名 struct {
//	结构体成员列表
//	成员名 数据类型
//	姓名 string
//	age int
//}

//结构体是全局的可以在项目所有文件使用
//结构体是一种数据类型
type student struct {
	id    int
	name  string
	age   int
	sex   string
	score int
	addr  string
}

func main0901() {

	//定义结构体变量 复合类型
	//var 变量名 结构体名
	//var stu student
	//
	////为结构体成员赋值 包名.函数名 结构体.成员  对象.方法
	//stu.name = "张大帅"
	//stu.score = 99
	//stu.addr = "奉天皇姑"
	//stu.sex = "男"
	//stu.age = 58
	//stu.id = 1

	//定义结构体是为成员赋值
	//var stu student = student{1, "张宗昌", 49, "男", 5, "山东济南"}
	//自动推导类型和指定成员赋值
	stu := student{name: "孙殿英", score: 60, sex: "男", addr: "北平", id: 1, age: 42}
	fmt.Println(stu)
}
func main0902() {
	stu := student{101, "朱德", 60, "男", 101, "四川"}

	fmt.Printf("%p\n", &stu)
	fmt.Printf("%p\n", &stu.id)
	//结构体成员为string 需要和结构体最大的数据类型进行对齐
	fmt.Printf("%p\n", &stu.name)
	fmt.Printf("%p\n", &stu.age)
	fmt.Printf("%p\n", &stu.sex)
	fmt.Printf("%p\n", &stu.score)
	fmt.Printf("%p\n", &stu.addr)
}

func main() {
	stu := student{102, "聂荣臻", 60, "男", 101, "湖南"}

	//将结构体变量赋值
	stu1 := stu
	stu1.id = 103
	//
	//fmt.Println(stu1)
	//fmt.Println(stu)

	//两个结构体比较 是比较所有成员 如果成员相同 结果为真  支持== !=比较操作
	if stu1 == stu {
		fmt.Println("相同")
	} else {
		fmt.Println("不相同")
	}

}
