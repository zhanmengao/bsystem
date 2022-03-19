package main

import "fmt"

//结构体嵌套结构体
//父类
type Person struct {
	id   int
	name string
	age  int
	sex  string
}

//每一个类描述一个对象的属性
//子类
type Student struct {
	//将Person结构体作为student结构体的成员
	//p Person//为person结构体起名字
	Person //匿名字段实现继承关系

	class int
	score int
}

func main0101() {

	//创建对象
	var stu Student

	//student继承与父类person 可以直接使用父类的结构体成员
	//stu.Person.name = "张三"
	//stu.Person.age = 18
	//stu.Person.id = 1001
	stu.name = "张三"
	stu.age = 18
	stu.id = 1001
	stu.sex = "女"

	//stu.p.name
	//stu.p.age
	//stu.p.id
	//stu.p.sex
	stu.class = 302
	stu.score = 99

	fmt.Println(stu)

}
func main(){
	//如果一个结构体包含其他结构体信息 需要依次进行初始化
	var stu Student=Student{Person:Person{1001,"东方不败",35,"不祥"},score:100,class:302}
	fmt.Printf("姓名：%s\n",stu.name)
	fmt.Printf("性别：%s\n",stu.sex)
	fmt.Printf("年龄：%d\n",stu.age)
	fmt.Printf("编号：%d\n",stu.id)
	fmt.Printf("班级：%d\n",stu.class)
	fmt.Printf("成绩：%d\n",stu.score)
}