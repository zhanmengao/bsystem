package main

import "fmt"

func main0601() {

	//var arr [10]int
	//指针数组  数组元素是指针类型
	a := 10
	b := 20
	c := 30
	var arr [3]*int = [3]*int{&a, &b, &c}

	//fmt.Println(arr)
	//
	//fmt.Println(&a)
	//fmt.Println(&b)
	//fmt.Println(&c)

	//*arr[1] = 200

	//fmt.Println(b)

	for i := 0; i < len(arr); i++ {
		fmt.Println(*arr[i])
	}
}
func main() {
	a := [3]int{1, 2, 3}
	b := [3]int{4, 5, 6}
	c := [3]int{7, 8, 9}

	//p := &a

	//二维数组模型
	var arr [3]*[3]int = [3]*[3]int{&a, &b, &c}

	fmt.Println(arr)
	//fmt.Printf("%T\n", p)

	//for i := 0; i < len(arr); i++ {
	//	//fmt.Println(*arr[i])//[3]int类型
	//	for j := 0; j < len(*arr[i]); j++ {
	//		fmt.Printf("%d ", (*arr[i])[j])
	//	}
	//	fmt.Println()
	//}

	(*arr[1])[1] = 555

	fmt.Println(b)
}
