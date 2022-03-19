package main

import "fmt"

//...不定参 在函数调用时可以传递不定量（0-n）的参数
//不定参使用数据格式为切片
func sum(arr ...int) {
	//arr是一个数据的集合
	//fmt.Println(arr)

	//计算字符串个数
	//len("你好")
	//计算数据集合的个数
	//count := len(arr)
	//fmt.Println(count)

	//通过arr[下标]可以找到具体数据的值
	//下标是从0开始的  到len(arr)-1为最大值下标
	//fmt.Println(arr[0])
	//fmt.Println(arr[1])
	//fmt.Println(arr[2])
	//fmt.Println(arr[3])

	sum := 0
	//通过for循环遍历集合中的数据
	//for i := 0; i < len(arr); i++ {
	//	//fmt.Println(arr[i])
	//	sum += arr[i]
	//}
	//通过for循环遍历集合中的数据
	//i -> index 下标 v -> value 值
	//_匿名变量  如果数据的值不需要接收  可以通过匿名变量来接收数据
	for _, v := range arr {
		//fmt.Println(i, v)
		sum += v
	}
	fmt.Println(sum)
}
func main() {
	//不定参函数调用
	sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
	sum(12, 21, 23, 32)

	//fmt.Println 就是一个不定参的格式
	fmt.Println("你好", 123, "再见")
}
