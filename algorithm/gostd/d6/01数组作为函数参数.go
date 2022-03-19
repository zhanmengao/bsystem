package main

import "fmt"

func swap(a int, b int) {
	a, b = b, a
}

func main0101() {
	a := 10
	b := 20
	//值传递  形参不能改变实参的值  形参和实参是不同的地址单元
	swap(a, b)

	fmt.Println(a)
	fmt.Println(b)
}

//数组作为函数参数  返回值
func BubbleSort(arr [10]int)  [10]int {

	//fmt.Println(arr)
	//fmt.Println(len(arr))

	//arr[0]=123
	//arr[1]=234

	for i := 0; i < len(arr)-1; i++ {
		for j := 0; j < len(arr)-1-i; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}
		}
	}

	return arr
	//fmt.Println(arr)

}
func main() {
	arr := [10]int{9, 1, 5, 6, 8, 4, 7, 10, 3, 2}
	//数组作为函数参数是值传递
	//形参和实参是不同的存储单元
	// 内存中有两份独立的数组存储不同的数据
	// 在函数调用结束形参单元销毁 不会影响主调函数中实参的值

	//如果想通过函数计算结果并传递给实参 需要使用数组作为函数的返回值
	arr = BubbleSort(arr)

	fmt.Println(arr)

}
