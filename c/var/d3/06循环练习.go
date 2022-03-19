package main

import "fmt"

func main0601() {

	//敲7 7的倍数 个位为7 十位为7 需要敲桌子  1-100

	for i := 1; i <= 100; i++ {
		if i%7 == 0 || i%10 == 7 || i/10 == 7 {
			fmt.Println("敲桌子")
		} else {
			fmt.Println(i)
		}
	}

}

func main0602() {
	//水仙花数  一个三位数 各个位数的立方和等于这个数本身
	for i := 100; i <= 999; i++ {
		//百位
		a := i / 100
		//十位
		b := i / 10 % 10 //b:=i%100/10
		//个位
		c := i % 10

		if a*a*a + b*b*b + c*c*c == i {
			fmt.Println(i)
		}
	}

}
