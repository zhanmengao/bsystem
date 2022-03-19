package main

import "fmt"

func main0101() {

	//	数组定义
	// 数组是一系列相同数据类型在内存中有序存储的数据集合
	// var 数组名 [元素个数]数据类型
	//定义了10个整型变量的数组元素
	var arr [10]int

	//通过下标找到具体元素  数组下标是从0开始的  到数组元素个数-1位数值最大下标
	//数组名[下标]  指向具体某一个元素
	arr[0] = 123
	arr[1] = 110
	arr[2] = 234

	arr[5] = 567
	//打印数组
	fmt.Println(arr)

	fmt.Println(arr[0])
	fmt.Println(arr[1])
	fmt.Println(arr[2])

}

func main0102() {
	//在定义数组时  依次为数组元素赋值
	//var arr [10]int = [10]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	//在定义数组是 可以为部分元素赋值
	//var arr [10]int = [10]int{1, 2, 3, 4, 5}
	//在定义数组时可以指定下标赋值
	//var arr [10]int = [10]int{1: 10, 4: 20, 6: 30}

	//定义时写...可以根据元素个数赋值
	//var arr [4]int = [...]int{1, 2, 3, 4}
	arr := [10]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	//len(数组名)//计算数组元素个数
	//fmt.Println(len(arr))

	//遍历数组元素值
	//for i:=0;i<len(arr);i++{
	//	fmt.Println(arr[i])
	//}

	//可以使用range 数组名变量
	//i -》index 下标 v-》value 值
	for _, v := range arr {
		fmt.Println(v)
	}
	//fmt.Println(arr)
}

func main() {
	//数组常见问题

	//i是一个变量  数组元素个数定义一个是常量 或是常量表达式
	//i:=10
	//const i =10
	var arr [10]int = [10]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	//var arr1 [10]int

	//arr[0]=123

	//数组下标越界 最大值下标：len(arr)-1
	//arr[10]=321//err
	//arr[-1]=321//err

	//arr是数组名  表示的是一个数组
	//arr=123err

	//两个数组如果类型和元素个数相同可以赋值
	//arr1=arr

	fmt.Println(arr)
	//fmt.Println(arr1)

	//打印数组数据类型
	fmt.Printf("%T\n", arr)
	//fmt.Printf("%T\n",arr1)

	//打印数组地址
	//数组名表示整个数组 数组名对应的地址就是数组第一个元素的地址
	fmt.Printf("数组地址：%p\n", &arr)
	//fmt.Printf("数组第一个元素地址：%p\n",&arr[0])
	//fmt.Printf("数组第二个元素地址：%p\n",&arr[1])
	//fmt.Printf("数组第三个元素地址：%p\n",&arr[2])

	//打印数组所有元素地址
	for i := 0; i < len(arr); i++ {
		fmt.Printf("%p\n", &arr[i])
	}

}
