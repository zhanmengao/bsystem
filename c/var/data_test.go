package main

import (
    "fmt"
    "testing"
) //include stdio.h

//第一种，指定变量类型，如果没有初始化，则变量默认为零值。
//var v_name v_type
//v_name = value
func f1(){
	 // 声明一个变量并初始化
    var a = "RUNOOB"
    fmt.Println(a)

    // 没有初始化就为零值
    var b int
    fmt.Println(b)

    // bool 零值为 false
    var c bool
    fmt.Println(c)
	
	var i int
    var f float64
    var b2 bool
    var s string
    fmt.Printf("%v %v %v %q\n", i, f, b2, s)
}

//第二种，根据值自行判定变量类型。	var v_name = value
func f2(){
	 var d = true
    fmt.Println(d)
}

//第三种，省略 var, 注意 := 左侧如果没有声明新的变量，就产生编译错误，格式：v_name := value
func f3(){
	f := "Runoob" 							// var f string = "Runoob"
    fmt.Println(f)
}

func TestData(t *testing.T) {
    var a string = "Runoob"
    fmt.Println(a)

    var b, c int = 1, 2
    fmt.Println(b, c)
}
/*
1	布尔型
布尔型的值只可以是常量 true 或者 false。一个简单的例子：var b bool = true。
2	数字类型
整型 int 和浮点型 float32、float64，Go 语言支持整型和浮点型数字，并且支持复数，其中位的运算采用补码。
3	字符串类型:
字符串就是一串固定长度的字符连接起来的字符序列。Go 的字符串是由单个字节连接起来的。Go 语言的字符串的字节使用 UTF-8 编码标识 Unicode 文本。
4	派生类型:
包括：

    (a) 指针类型（Pointer）
    (b) 数组类型
    (c) 结构化类型(struct)
    (d) Channel 类型
    (e) 函数类型
    (f) 切片类型
    (g) 接口类型（interface）
    (h) Map 类型 

所有像 int、float、bool 和 string 这些基本类型都属于值类型，使用这些类型的变量直接指向存在内存中的值：

 你可以通过 &i 来获取变量 i 的内存地址，例如：0xf840000040（每次的地址都可能不一样）。值类型的变量的值存储在栈中。
内存地址会根据机器的不同而有所不同，甚至相同的程序在不同的机器上执行后也会有不同的内存地址。因为每台机器可能有不同的存储器布局，并且位置分配也可能不同。

更复杂的数据通常会需要使用多个byte，这些数据一般使用引用类型保存。
一个引用类型的变量 r1 存储的是 r1 的值所在的内存地址（数字），或内存地址中第一个字所在的位置。 
 这个内存地址称之为指针，这个指针实际上也被存在另外的某一个值中。
同一个引用类型的指针指向的多个字可以是在连续的内存地址中（内存布局是连续的），这也是计算效率最高的一种存储形式；也可以将这些字分散存放在内存中，每个字都指示了下一个字所在的内存地址。
当使用赋值语句 r2 = r1 时，只有引用（地址）被复制。
如果 r1 的值被改变了，那么这个值的所有引用都会指向被修改后的内容，在这个例子中，r2 也会受到影响。 
*/

