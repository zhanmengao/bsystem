package main

import "fmt"

func main() {
	//通过键盘输入一个英文字符串 统计每个字母出现的次数
	//helloworld
	//helowrd
	//1232111

	var str string

	fmt.Scan(&str)
	//将字符串转成切片
	slice := []byte(str)

	//定义map进行数据存储
	//map[字符]个数
	m := make(map[byte]int)

	//var ch byte
	for i := 0; i < len(slice); i++ {
		//ch = slice[i]
		//m[ch] += 1
		//将slice切片的中的元素作为m中的key
		m[slice[i]]++
	}

	for k, v := range m {
		fmt.Printf("%c:%d\n", k, v)
	}

}
