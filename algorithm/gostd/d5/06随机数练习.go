package main

import (
	"math/rand"
	"time"
	"fmt"
)

func main0601() {

	rand.Seed(time.Now().UnixNano())
	var arr [10]int

	for i := 0; i < len(arr); i++ {
		arr[i] = rand.Intn(100)
	}

	fmt.Println(arr)

	for i := 0; i < len(arr)-1; i++ {
		for j := 0; j < len(arr)-1-i; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}

		}
	}
	fmt.Println(arr)
}

func main0602() {
	//猜数字 1-100

	rand.Seed(time.Now().UnixNano())

	num := rand.Intn(100) + 1

	value := 0
	for {
		fmt.Println("请输入一个数字：")
		fmt.Scan(&value)

		if num > value {
			fmt.Println("您输入的数字太小了")
		} else if num < value {
			fmt.Println("您输入的数组太大了")
		} else {
			fmt.Println("您猜对了")
			break
		}
	}

}

//数组去重
func main() {
	//随机双色球彩票
	//红色 1-33  选择6个 不能重复 蓝球 1-16 选择1个 可以和红球重复

	rand.Seed(time.Now().UnixNano())
	var red [6]int

	for i := 0; i < len(red); i++ {
		v := rand.Intn(33) + 1

		for j := 0; j < i; j++ {
			//数据重复
			if v == red[j] {
				//重新随机
				v = rand.Intn(33) + 1
				//将j赋值为-1在循环执行到上面是进行++操作后值为0  也就相当于重新比对
				j = -1
			}
		}
		//将没有重复的数字添加到数组中
		red[i] = v

	}
	fmt.Println("红球：", red, "蓝球：", rand.Intn(16)+1)
}
