package main

import "fmt"

func main0801() {

	//数组定义
	//var 数据名 [元素个数]数据类型

	//切片定义
	//var 切片名 []数据类型

	var slice []int = []int{1, 2, 3, 4, 5}
	slice[0] = 123
	slice[2] = 234
	//在使用切片是不能超过切片的范围  下标越界
	//slice[5] = 456 //err

	//append 切片添加数据

	slice = append(slice, 456)
	slice = append(slice, 7, 8, 9)

	fmt.Println(slice)
	//len(切片名)计算切片的长度
	for i := 0; i < len(slice); i++ {
		fmt.Println(slice[i])
	}

	//for i,v:=range slice{
	//	fmt.Println(i,v)
	//}
}
func main() {
	//切片赋值
	//var slice []int
	//slice[0]=123//err切片下标越界

	//slice = append(slice, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

	//在定义切片时可以指定长度  make(切片类型，长度)
	//var slice []int=make([]int,10)
	//
	//slice[0]=123
	//slice[1]=234
	//slice = append(slice, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

	//通过自动推到类型创建切片
	slice :=[]int{1,2,3}

	fmt.Println(slice)
	fmt.Println(len(slice))
}
