package main

import "fmt"

func main() {
	var arr [10]int = [10]int{9, 1, 5, 6, 8, 2, 10, 7, 4, 3}

	//外层执行一次内层执行一周
	//外层控制行
	for i := 0; i < len(arr)-1; i++ {
		//内层控制列
		for j := 0; j < len(arr)-1-i; j++ {

			//满足条件进行交换  大于号 升序  小于号 降序
			if arr[j] < arr[j+1] {
				//交换数据
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}

		}
	}

	fmt.Println(arr)

}
