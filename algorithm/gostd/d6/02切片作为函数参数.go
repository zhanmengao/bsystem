package main

import "fmt"

//切片名作为函数参数 返回值
func test(s []int) []int{
	//fmt.Println(s)
	//fmt.Println(len(s))
	//s[0]=123
	//s[5]=666
	//fmt.Printf("%p\n", s)

	//for i := 0; i < len(s)-1; i++ {
	//	for j := 0; j < len(s)-1-i; j++ {
	//		if s[j] > s[j+1] {
	//			s[j], s[j+1] = s[j+1], s[j]
	//		}
	//	}
	//}

	//如果是函数中使用append进行数据添加时 形参的地址改变就不会在指向实参的地址

	s = append(s, 11, 12, 13, 14, 15)
	fmt.Printf("%p\n",s)
	fmt.Println(s)
	return s
}
func main() {
	//切片名本身就是一个地址
	slice := []int{9, 1, 5, 6, 8, 4, 7, 10, 3, 2}
	fmt.Printf("%p\n", slice)
	//地址传递  形参可以改变实参
	slice=test(slice)

	fmt.Println(slice)
}
