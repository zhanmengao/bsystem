package main

import "fmt"

func main0201() {
	//byte字符类型 同时也是uint8的别名
	var a byte = 'a'

	//所有的字符都对应ASCII中的整型数据
	//'0'对应的48  'A'对应的65 'a' 对应的97
	//fmt.Println(a)

	//%c是一个占位符 表示打印输出一个字符
	fmt.Printf("%c\n", a)
	fmt.Printf("%c\n", 97)

	fmt.Printf("%T\n", a)
	var b byte = '0' //字符0  对应的ASCII值为为48

	fmt.Printf("%c\n", 48)
	fmt.Printf("%c\n", b)
}
func main0202() {

	var a byte = 'a'
	//将小写字母转成大写字母输出
	fmt.Printf("%c", a-32)
}

func main() {
	//转义字符 \n 换行

	//var a byte = '\n'
	//\0  对应的ASCII 值为0 用于字符串的结束标志
	//\t 对应的ASCII 值为9 水平制表符 一次跳八个空格
	var a byte ='\t'
	//fmt.Println(a)
	fmt.Printf("%c",a)


}
