package gostb

import(
	"fmt"
)

type vector struct{
	arr []int
	size int
	cap int
}
func(v *vector)PushBack(val int){
	if v.arr == nil{
		v.arr = make([]int,4)
		v.size = 0
		v.cap = 4
	}
	if v.size+1 >= v.cap{
		z:= make([]int,v.cap*2)
		copy(z,v.arr)
		v.arr = z
		v.cap *=2
	}
	v.arr[v.size] = val
	v.size++
}
func(v *vector)Print(){
	for i:=0;i<v.size;i++{
		fmt.Printf("arr[%d] = %d \n",i,v.arr[i])
	}
}