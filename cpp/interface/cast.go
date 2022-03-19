package main

import "fmt"

func main() {
	var num int64 = 99
	getNum(num)

	var str = "kkjhh"
	getStr(str)
}

func getNum(iNum interface{}) {
	num := iNum.(int64)
	fmt.Println(num)
}

func getStr(iStr interface{}) {
	str := iStr.(string)
	fmt.Println(str)
}