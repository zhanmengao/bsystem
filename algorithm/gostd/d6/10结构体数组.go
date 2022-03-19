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

//结构体数组作为函数参数
func Sort(arr []Student) {

	for i := 0; i < len(arr)-1; i++ {
		for j := 0; j < len(arr)-1-i; j++ {
			//比较结构体成员信息
			if arr[j].age > arr[j+1].age {
				//交换数组元素
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}
		}
	}

	//fmt.Println(arr)
}
func main() {

	//结构体数组 |  切片
	var arr []Student = []Student{
		Student{101, "唐三藏", 32, "男", 100, "东土大唐"},
		Student{102, "孙悟空", 532, "男", 100, "花果山"},
		Student{103, "猪悟能", 600, "男", 100, "高老庄"},
		Student{104, "沙悟净", 180, "男", 100, "流沙河"},
		Student{105, "白龙马", 10, "男", 100, "西海"}}

	//fmt.Println(arr)

	//打印结构体信息
	//for i := 0; i < len(arr); i++ {
	//	fmt.Println(arr[i])
	//}
	//修改指定结构体成员的信息
	//arr[1].score = 60
	//arr[2].score = 10
	//fmt.Println(arr)

	//结构体切片添加数据
	arr = append(arr, Student{106, "如来大魔王", 9999, "男", 100, "西天"})

	//结构体数组作为函数参数是值传递
	//结构体切片作为函数参数是地址传递（引用传递）
	Sort(arr)
	fmt.Println(arr)
}
