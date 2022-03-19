package main

import "fmt"

func main0501() {

	arr := [10]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	//定义指针 指向数组  数组指针
	//自动推导类型创建数组指针
	//p := &arr

	//指针和数组建立关系
	var p *[10]int
	p = &arr

	//fmt.Printf("%T\n", p)

	//通过指针间接操作数组

	//fmt.Println(len(p))
	for i := 0; i < len(p); i++ {
		fmt.Println(p[i])
	}
	//(*数组指针变量)[下标]=值
	//(*p)[1] = 222
	//数组指针可以直接操作数组元素
	//p[1] = 222//ok
	//fmt.Println(*p)
}

func main0502() {
	arr := [5]int{1, 2, 3, 4, 5}
	//指针变量和要存储的数据类型要相同

	//p1和p2在内存中指向相同的地址  但是p1和p2的类型不同 一个是指向整个数组   一个是指向数组的元素
	p1 := &arr
	//p2 := &arr[0]

	//fmt.Printf("%p\n", p1)
	//fmt.Printf("%p\n", p2)
	//
	//fmt.Printf("%T\n", p1)
	//fmt.Printf("%T\n", p2)

	for i := 0; i < len(p1); i++ {
		fmt.Println(p1[i])
	}

}

//将数组指针作为函数参数

//func test2(p *[5]int) {
//
//	//通过指针间接操作数组
//	p[2] = 123
//
//}
//将数组指针作为函数参数
func BubbleSort(arr *[10]int) {

	for i := 0; i < len(arr)-1; i++ {
		for j := 0; j < len(arr)-1-i; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}
		}
	}
}
func main() {
	arr := [10]int{9, 1, 5, 6, 8, 4, 10, 7, 2, 3}
	//p := &arr
	//地址传递
	//test2(p)

	BubbleSort(&arr)
	fmt.Println(arr)
}
