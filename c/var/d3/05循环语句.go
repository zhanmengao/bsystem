package main

import "fmt"

func main() {

	//for i := 0; i < 5; i++ {
	//	fmt.Println("传智播客")
	//}

	//for i := 1; i <= 10; i++ {
	//	fmt.Println(i)
	//}

	//计算1-100和
	//sum := 0
	//for i := 1; i <= 100; i++ {
	//	sum += i
	//}
	//
	//fmt.Println(sum)

	//计算1-100偶数的和
	//在for语句中嵌套if条件判断
	//sum := 0
	//for i := 1; i <= 100; i++ {
	//	if i%2 == 0 {
	//		sum+=i
	//	}
	//}
	//
	//fmt.Println(sum)
	//计算1-100偶数的和
	sum := 0
	for i := 0; i <= 100; i += 2 {
		sum += i
	}

	fmt.Println(sum)

}
