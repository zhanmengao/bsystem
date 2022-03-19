package main

import "fmt"

func main0801() {
	/*
	中国古代数学家张丘建在他的《算经》中提出了一个著名的“百钱百鸡问题”：
	一只公鸡值五钱，一只母鸡值三钱，三只小鸡值一钱，
	现在要用百钱买百鸡，请问公鸡、母鸡、小鸡各多少只？
	 */

	//cock hen chicken
	count := 0
	for cock := 0; cock <= 20; cock++ {
		for hen := 0; hen <= 33; hen++ {
			for chicken := 0; chicken <= 100; chicken += 3 {
				count++
				//三种鸡个数为100 价格为100
				if cock+hen+chicken == 100 && 5*cock+3*hen+chicken/3 == 100 {
					fmt.Printf("公鸡：%d  母鸡：%d  小鸡：%d\n", cock, hen, chicken)
				}

			}
		}

	}

	fmt.Println("执行次数", count)
}
func main() {

	count:=0
	for cock := 0; cock <= 20; cock++ {
		for hen := 0; hen <= 33; hen++ {
			count++
			//小鸡个数为100-公鸡-母鸡
			chicken := 100 - cock - hen
			if chicken%3==0 && 5*cock+3*hen+chicken/3 == 100 {
				fmt.Printf("公鸡：%d  母鸡：%d  小鸡：%d\n", cock, hen, chicken)

			}
		}
	}

	fmt.Println(count)
}
//72114  24308 714