package main

import "fmt"

type Person struct {
	id   int
	name string
	sex  string
	age  int
	addr string
}

func main0801() {

	var per Person = Person{101, "张三疯", "男", 180, "武当山"}

	//fmt.Printf("%p\n", &per)

	//定义指针接收结构体变量地址
	//p := &per
	var p *Person = &per
	//fmt.Printf("%T\n", p) //*Person 类型

	//通过指针间接修改结构体成员的值
	(*p).name = "张君宝"
	//指针可以直接操作结构体成员
	p.age = 140
	p.id = 1000

	fmt.Println(per)
}

//结构体指针作为函数参数
func test4(p *Person) {
	p.addr = "斯塔克公司"
}
func main0802() {
	//结构体指针作为函数参数
	var per Person = Person{101, "钢铁侠", "男", 180, "武当山"}

	//地址传递 形参可以改变实参的值
	test4(&per)

	fmt.Println(per)
}

func main0803() {
	arr := [3]Person{Person{101, "钢铁侠", "男", 180, "斯塔克公司"},
		Person{102, "绿巨人", "男", 38, "实验室"},
		Person{103, "黑寡妇", "女", 23, "前苏联"}}
	//指向结构体数组的指针
	p := &arr

	fmt.Printf("%p\n", p)

	//p[0].age = 40//ok
	(*p)[0].age = 40 //ok

	for i := 0; i < len(p); i++ {
		fmt.Println(p[i])
	}

}

func main() {
	//map类型变量
	m := make(map[int]*[3]Person)

	//fmt.Printf("%T\n", m)
	m[1] = new([3]Person) //返回值类型为*[3]Person
	m[1] = &[3]Person{Person{101, "钢铁侠", "男", 180, "斯塔克公司"},
		Person{102, "绿巨人", "男", 38, "实验室"},
		Person{103, "黑寡妇", "女", 23, "前苏联"}}

	m[2] = new([3]Person)
	m[2] = &[3]Person{Person{101, "索尔", "男", 180, "斯塔克公司"},
		Person{102, "美队", "男", 38, "实验室"},
		Person{103, "黑豹", "女", 23, "前苏联"}}

	for k, v := range m {
		fmt.Println(k, *v)
	}

	////数组指针
	//var p *[3]int
	//
	////创建内存空间存储[3]int
	//
	//p = new([3]int)
	//
	//p[0] = 123
	//p[1] = 222
	//p[2] = 666
	//fmt.Println(p)
}
