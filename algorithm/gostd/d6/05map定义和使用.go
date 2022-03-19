package main

import "fmt"

func main0501() {

	//map[key]value
	//map[键类型]值类型

	//内存地址编号为0的空间是系统占用  不允许进行读写操作
	//var m map[int]string  //内存地址编号为0x0
	//map存储的方式不是顺序存储的
	var m map[int]string = map[int]string{101: "法师", 251: "张超", 666: "怡红"}
	//m[101]="法师"

	//fmt.Printf("%p\n", m)

	//fmt.Println(m)
	//k->key v->value
	//for k, v := range m {
	//	fmt.Println(k, v)
	//}
	//找到具体的值
	fmt.Println(m[666])
}


func main(){
	//make(map[key]value) make(map[key]value,长度)
	m:=make(map[string]int)

	//len(map)计算map大小
	fmt.Println(len(m))
	//map的长度是自动扩容的
	//在map中key是唯一的  值可以重复
	m["法师"]=101
	fmt.Println(len(m))
	m["小明"]=110
	fmt.Println(len(m))
	m["乾隆"]=120
	//重新赋值
	//m["小明"]=666//ok
	fmt.Println(len(m))

	fmt.Printf("%p\n",m)
	fmt.Println(m)
}