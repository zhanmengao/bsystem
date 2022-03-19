package main

import "fmt"

func main0901() {

	//切片名本身就是一个地址
	//创建的空切片 指向内存地址编号为0的空间
	var slice []int

	fmt.Printf("%p\n", slice)

	//当使用append进行追加数据时 切片地址可能会发生改变
	slice = append(slice, 1, 2, 3)

	fmt.Printf("%p\n", slice)

	slice = append(slice, 4, 5, 6)

	fmt.Printf("%p\n", slice)

}

func main0902() {
	//var slice []int =make([]int,10)
	//make(数据类型，长度，容量)
	slice := make([]int, 10, 20)

	//在使用appned进行数据添加时  如果长度超出容量  容量会自动扩容
	//一般扩容方式为上一次 容量*2  如果超过1024字节 每次扩容上一次的1/4
	//容量扩容每次都是偶数
	//len(slice)  计算切片的长度
	//cap(slice)  计算切片的容量
	fmt.Println(cap(slice))
	fmt.Printf("%p\n", slice)
	fmt.Println(slice)

	slice = append(slice, 1, 2, 3, 4, 5)
	fmt.Println(cap(slice))
	fmt.Printf("%p\n", slice)
	fmt.Println(slice)

	slice = append(slice, 1, 2, 3, 4, 5, 6)
	fmt.Println(cap(slice))
	fmt.Printf("%p\n", slice)
	fmt.Println(slice)
}
func main() {
	slice := []int{1, 2, 3, 4, 5}

	fmt.Println("长度", len(slice))
	fmt.Println("容量", cap(slice))

	slice = append(slice, 1)
	fmt.Println("长度", len(slice))
	fmt.Println("容量", cap(slice))

	//使用len(slice)来打印数据 不能使用cap(slice)
	//for i := 0; i < cap(slice); i++ {
	//	fmt.Println(slice[i])
	//}
}
