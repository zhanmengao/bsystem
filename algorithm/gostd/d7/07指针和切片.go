package main

import "fmt"

func main0701() {
	slice := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	//指针和切片建立关系
	p := &slice
	//fmt.Printf("%p\n", p)
	//fmt.Printf("%p\n", slice)

	//*[]int
	//fmt.Printf("%T\n", p)
	//因为指向切片的指针 加下标找不到具体元素
	//p[1] = 222//err

	//(*p)[1] = 222

	//fmt.Println(slice)

	//通过指针间接操作切片的元素
	for i := 0; i < len(*p); i++ {
		fmt.Println((*p)[i])
	}
}

//func test2(s []int) {
//
//	s = append(s, 1, 2, 3)
//	fmt.Println(s)
//}

//切片指针作为函数参数
func test3(s *[]int) {

	*s = append(*s, 1, 2, 3)
}
func main() {
	s := []int{1, 2, 3, 4, 5, 6}

	//test2(s)
	//地址传递
	test3(&s)
	fmt.Println(s)
}
