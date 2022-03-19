package main

import "fmt"

//将map作为函数参数
func demo(m map[int]string) {

	m[4] = "刘胡兰"
	m[5] = "赖宁"
	fmt.Println(m)
	//fmt.Printf("%p\n",m)
	delete(m, 5)
	fmt.Println(m)

}
func main() {

	m := map[int]string{1: "雷锋", 2: "董存瑞", 3: "邱少云"}
	//地址传递  引用传递  形参和实参指向内存中相同的地址段  修改形参会影响实参的值
	demo(m)
	fmt.Println(m)
	//fmt.Printf("%p\n",m)

}
