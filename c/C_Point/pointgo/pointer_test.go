package main

import (
	"fmt"
	"testing"
	"unsafe"
)




func TestPointer(t *testing.T) {
	bytes := []byte{104, 101, 108, 108, 111}

	p := unsafe.Pointer(&bytes) //强制转换成unsafe.Pointer，编译器不会报错
	str := *(*string)(p)        //然后强制转换成string类型的指针，再将这个指针的值当做string类型取出来
	fmt.Println(str)            //输出 "hello"
}


type user struct {
	name string
	age  uint
}

func TestUintptr(t *testing.T) {
	// 定义一个指针变量
	student := new(user)
	// user 结构体中的 name 字段是第一个字段，可以直接通过指针修改，不需要使用偏移
	studentName := (*string)(unsafe.Pointer(student))
	*studentName = "lucy"
	// user 结构体中的 age 字段不是第一个字段，所以需要使用偏移才能找到 age 字段的内存地址，修改值
	studentAge := (*uint)(unsafe.Pointer(uintptr(unsafe.Pointer(student)) + unsafe.Offsetof(student.age)))
	*studentAge = 18
	fmt.Println(*student)
}

/*
我们使用 new 函数定义了一个 *user 类型的指针变量 student，然后使用 unsafe.Pointer 将 *user 类型的指针变量 student 转换为 *string 类型的指针变量 studentName，
然后修改 studentName 的值，实际上就是修改 name 字段的值。

因为 age 字段不是 user 结构体的第一个字段，所以需要先使用偏移量找到 age 字段的内存地址，具体操作步骤是：先将 student 指针变量通过 unsafe.Pointer 和 uintptr 转换为 uintptr，
然后就可以使用函数 unsafe.Offsetof 计算出 age 字段的偏移量，该函数返回结果也是 uintptr 类型，因为 uintptr 支持运算，最后使用 + 运算符获取 age 字段的内存地址。

找到 age 字段的内存地址之后，还要使用 unsafe.Pointer 转换为 *uint 指针类型，才可以对该内存地址进行读写操作。
*/

