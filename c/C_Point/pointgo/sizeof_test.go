package main

import (
	"fmt"
	"testing"
	"unsafe"
)
type s1 struct{
	a bool			//0
	b int16			//2
	c []int			//8 16 24 = 32
}
type s2 struct{
	c bool			//0
	a float64		//8
	b int16			//16  对齐=24
}
type s3 struct{
	a float64		//0
	b int16			//8
	c bool			//10 对齐=16
}

func TestSizeof(t *testing.T) {
	fmt.Println("sizeof(bool)",unsafe.Sizeof(bool(false)))
	fmt.Println("sizeof(int32)",unsafe.Sizeof(int32(0)))
	fmt.Println("sizeof(int64)",unsafe.Sizeof(int64(0)))
	fmt.Println("sizeof(uintptr)",unsafe.Sizeof(uintptr(0)))
	fmt.Println("sizeof(void*)",unsafe.Sizeof(&s1{}))
	fmt.Println("sizeof(string)",unsafe.Sizeof(string("112333")))
	fmt.Println("sizeof(slice)",unsafe.Sizeof(make([]int,0)))
	fmt.Println("sizeof(map)",unsafe.Sizeof(make(map[string]int)))
	fmt.Println("sizeof(func)",unsafe.Sizeof(TestSizeof))
	fmt.Println("sizeof(interface)",unsafe.Sizeof(interface{}(false)))
	fmt.Println("sizeof(chan)",unsafe.Sizeof(make(chan struct{})))
	fmt.Println("sizeof(struct)",unsafe.Sizeof(s1{}))
	fmt.Println("sizeof(struct)",unsafe.Sizeof(s2{}))
	fmt.Println("sizeof(struct)",unsafe.Sizeof(s3{}))
}
