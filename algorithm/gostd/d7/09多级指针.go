package main

import "fmt"

func main0901() {
	a := 10
	//b := 20
	//  一级指针 指向变量的地址
	p := &a

	//二级指针 指向一级指针的地址
	var pp **int = &p

	//通过二级指针连接修改一级指针的值
	//*pp = &b
	//通过二级指针间接修改变量的值
	**pp = 100
	//var ppp ***int
	//var pppp ****int
	//pp := &p
	//*int
	fmt.Printf("%T\n", p)
	fmt.Printf("%T\n", pp)
}
func main0902() {
	a := 10

	var p *int = &a
	var pp **int = &p
	//三级指针指向二级指针的地址
	var ppp ***int = &pp
	//引用运算符 不能连续使用 如果放在一起成为 逻辑与

	//三级指针
	//ppp=&pp
	//二级指针
	//*ppp=pp=&p
	//一级指针
	//**ppp=*pp=p=&a
	//变量的值
	//***ppp=**pp=*p=a
}
