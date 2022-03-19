package main

import "fmt"

func main1001() {
	slice :=[]int{1,2,3,4,5,6,7,8,9,10}
	//切片名[起始位置：结束位置+1]  切片名[low:high]
	//s:=slice[3:7]
	//容量=max-low
	//切片名[起始位置:结束位置+1:容量]  切片名[low:high:max]
	//s:=slice[3:7:7]
	//切片名[起始位置:] 从起始位置打结束
	//s:=slice[2:]
	//切片名[:结束位置+1]
	//s:=slice[:5]
	//s:=slice[:]
	s:=slice
	fmt.Println(s)

	//容量要大于等于长度
	//fmt.Println(len(s))
	//fmt.Println(cap(s))
}

func main(){
	slice :=[]int{1,2,3,4,5,6,7,8,9,10}

	//截取后的切片还是原始切片中的一块内容 如果修改截取后的切片 影响原始切片的值
	//s:=slice[2:5]

	s:=slice[:]
	s[1]=123//3 123 5
	fmt.Println(s)

	fmt.Println(slice)

	fmt.Printf("%p\n",slice)
	fmt.Printf("%p\n",s)
}

