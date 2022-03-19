package main

import "fmt"

func main() {

	var arr [10]int

	//通过键盘为数组元素赋值
	for i := 0; i < len(arr); i++ {
		fmt.Scan(&arr[i])
	}

	//通过循环遍历数组
	max := arr[0]
	for i := 1; i < len(arr); i++ {
		if arr[i] > max {
			max = arr[i]
		}
	}

	fmt.Println(max)

}
