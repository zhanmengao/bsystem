package main

import "fmt"

func main0301() {

	//switch 变量(表达式) {
	//case 值1:
	//	代码体
	//  fallthrough
	//case 值2:
	//	代码体
	//default:
	//	代码体
	//}

	//根据分数 >=90 A >=80 B >=70 C >=60 D 不及格 E

	var score int
	fmt.Scan(&score)
	switch score / 10 {
	case 10:
		//fmt.Println("A")
		fallthrough
	case 9:
		fmt.Println("A")
	case 8:
		fmt.Println("B")
	case 7:
		fmt.Println("C")
	case 6:
		fmt.Println("D")
	default:
		fmt.Println("E")
	}
}
func main0302() {

	var score int
	fmt.Scan(&score)

	switch {
	case score >= 90:
		fmt.Println("A")
	case score >= 80:
		fmt.Println("B")
	case score >= 70:
		fmt.Println("C")
	case score >= 60:
		fmt.Println("D")
	default:
		fmt.Println("E")

	}
}
func main0303() {
	var score int
	fmt.Scan(&score)
	switch score > 60 {
	case true:
		fmt.Println("及格")
	case false:
		fmt.Println("不及格")
	}
}

func main() {
	//根据输入的年份月份 计算这个月有多少天

	var y int
	var m int
	fmt.Scan(&y, &m)

	//在switch语句中可以把相同的值放在一个case中
	switch m {
	case 1, 3, 5, 7, 8, 10, 12:
		fmt.Println(31)
	case 4, 6, 9, 11:
		fmt.Println(30)
	//fallthrough 在case中向下执行下一个case
	//case 1:
	//	fallthrough
	//case 3:
	//	fallthrough
	//case 5:
	//	fallthrough
	//case 7:
	//	fallthrough
	//case 8:
	//	fallthrough
	//case 10:
	//	fallthrough
	//case 12:
	//	fmt.Println(31)
	//
	//case 4:
	//	fallthrough
	//case 6:
	//	fallthrough
	//case 9:
	//	fallthrough
	//case 11:
	//	fmt.Println(30)
	case 2:
		//判断是否是闰年  能被4整除 但是 不能被100整除  或 能被400整除
		if y%4 == 0 && y%100 != 0 || y%400 == 0 {
			fmt.Println(29)
		} else {
			fmt.Println(28)
		}

	default:
		fmt.Println("月份输入错误")
	}
}

//case 3:
//	fmt.Println(31)
//case 4:
//	fmt.Println(30)
//case 5:
//	fmt.Println(31)
//case 6:
//	fmt.Println(30)
//case 7:
//	fmt.Println(31)
//case 8:
//	fmt.Println(31)
//case 9:
//	fmt.Println(30)
//case 10:
//	fmt.Println(31)
//case 11:
//	fmt.Println(30)
//case 12:
//	fmt.Println(31)

