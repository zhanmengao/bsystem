package main

import "fmt"

func main() {

	//iota枚举
	//iota枚举格式如果写在一行中值相等 如果换行值在上一行加一
	//const (
	//	a    = iota
	//	b, c = iota, iota
	//)

	//只需要对第一个进行iota赋值 后面会依次增长
	//const (
	//	a = iota
	//	b
	//	c
	//	d
	//)
	//在定义iota枚举时可以自定义赋值
	const (
		a = iota
		b = 10
		c = 20
		d
		e
		f = iota
		g
	)
	//a=100//err
	fmt.Println(a)
	fmt.Println(b)
	fmt.Println(c)
	fmt.Println(d)
	fmt.Println(e)
	fmt.Println(f)
	fmt.Println(g)
}
