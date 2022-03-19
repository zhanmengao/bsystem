package main

import (
	"fmt"
	"time"
)

func main0701() {
	//代码对齐 ctrl+alt+l
	count := 0
	//外层执行一次 内层执行一周
	for i := 0; i < 5; i++ {
		//fmt.Println(i)
		for j := 0; j < 5; j++ {
			count++
			fmt.Println(i, j)
		}
	}

	fmt.Println(count)
}

//电子时钟
func main0702() {
	//时
	for i := 0; i < 24; i++ {
		//分
		for j := 0; j < 60; j++ {
			//秒
			for k := 0; k < 60; k++ {
				//程序暂停
				time.Sleep(time.Millisecond * 950)
				//fmt.Println(i, j, k)
				fmt.Printf("%d 时 %d 分 %d 秒\n", i, j, k)
			}
		}
	}
}

//func main() {
//	fmt.Println(time.Now())
//	fmt.Println(time.Now().Second())
//	fmt.Println(time.Now().Day())
//	fmt.Println(time.Now().Month())
//	//这一面已经过了240天了
//	fmt.Println(time.Now().YearDay())
//}

func main() {
	//九九乘法口诀
	/*
	1*1=1
	1*2=2 2*2=4
	1*3=3 2*3=6 3*3=9
	 */
	// 外层控制行
	for i := 1; i <= 9; i++ {
		//内层控制列
		for j := 1; j <= i; j++ {
			//如果代码满足条件就跳出
			//if j > i {
			//	//跳出语句  可以跳出本层循环
			//	break
			//}
			fmt.Printf("%d*%d=%d\t", j, i, i*j)
		}
		fmt.Println()
	}

}

//打印等腰三角形
//     *
//    ***
//   *****
//  *******
// *********
//***********