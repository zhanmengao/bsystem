package main

import "fmt"

func main() {
	var slice []int = []int{1, 2, 3, 4, 5}

	s := make([]int, 5)
	//使用copy进行拷贝 在内存中存储两个独立的切片内容  如果任意一个发生修改不会影响另外一个
	copy(s, slice)

	fmt.Println(s)


	fmt.Printf("%p\n",slice)
	fmt.Printf("%p\n",s)
	//s[2]=123
	//fmt.Println(s)
	//fmt.Println(slice)
	//
	//
	//slice =append(slice,6,7,8)
	//fmt.Println(s)
	//fmt.Println(slice)
}
