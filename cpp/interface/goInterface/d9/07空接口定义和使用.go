package main

import "fmt"

func main0701() {

	var i interface{}

	//接口类型可以接收任意类型数据

	//fmt.Printf("%T\n", i)
	//i = 10
	//fmt.Printf("%T\n", i)
	//fmt.Println(i)
	//i = 3.14
	//fmt.Printf("%T\n", i)
	//fmt.Println(i)
	//i = "传智播客"
	//fmt.Printf("%T\n", i)
	//fmt.Println(i)

	i = 10

	//接口类型 不能直接进行转换需要使用类型断言
	//var a int =20
	//var a interface{}
	//a = 20

	fmt.Println(i)
}

func test() {
	fmt.Println("test hello world")
}
func main0702() {
	//空接口类型的切片
	var i []interface{}

	//fmt.Printf("%T\n",i)
	i = append(i, 10, 3.14, "传智播客", test)

	for idx := 0; idx < len(i); idx++ {
		fmt.Println(i[idx])
	}
}
