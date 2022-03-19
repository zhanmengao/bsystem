package main

import "fmt"

func main() {

	var arr [10]int = [10]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	//数组逆置
	i := 0            //最小值下标
	j := len(arr) - 1 //最大值下标


	//for 表达式1;表达式2;表达式3{}
	//for 返回值:=range 集合{}
	//for 条件 {}
	for i < j {
		//if (i >= j) {
		//	//跳出循环
		//	break
		//}

		//交换数据
		arr[i], arr[j] = arr[j], arr[i]
		//改变下标
		i++
		j--

	}

	fmt.Println(arr)

}
