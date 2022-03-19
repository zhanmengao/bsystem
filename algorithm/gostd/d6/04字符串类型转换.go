package main

import (
	"fmt"
	"strconv"
)

func main0401() {

	str:="hello world"

	//将字符串转成字符切片  强制类型转换
	slice:=[]byte(str)

	//fmt.Println(slice)


	slice[4]='a'
	for i:=0;i<len(slice);i++{
		fmt.Printf("%c",slice[i])
	}

}
func main0402(){
	//字符切片
	slice:=[]byte{'h','e','l','l','o',97}

	//fmt.Println(slice)

	fmt.Println(string(slice))
}
func main0403(){
	//将其他类型转成字符串  Format
	//b:=false
	//str:=strconv.FormatBool(b)
	//fmt.Println(str)


	//str:=strconv.FormatInt(123,10)//在计算机中进制可以表示2-36进制  2 8 10 16
	//fmt.Println(str)

	//str:=strconv.FormatFloat(3.14159,'f',4,64)
	//fmt.Println(str)


	str:=strconv.Itoa(123)
	fmt.Println(str)
}

func main0404(){
	//将字符串转成其他类型
	//b,err:=strconv.ParseBool("true")
	//if err != nil{
	//	fmt.Println("类型转化出错")
	//}else{
	//	fmt.Println(b)
	//}

	//value,_:=strconv.ParseInt("abc",16,64)
	//fmt.Println(value)

	//value,_:=strconv.ParseFloat("3.14159",64)
	//fmt.Println(value)


	//value,_:=strconv.Atoi("123")
	//
	//fmt.Println(value)
}

func main(){

	slice:=make([]byte,0,1024)
	//将其他类型转成字符串添加到字符切片里面

	slice=strconv.AppendBool(slice,false)
	slice=strconv.AppendInt(slice,123,2)

	slice=strconv.AppendFloat(slice,3.14159,'f',4,64)
	slice=strconv.AppendQuote(slice,"hello")

	fmt.Println(string(slice))
}