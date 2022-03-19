package main

import "fmt"

//打印等腰三角形
//     *		5 6-i-1		1 i*2+1
//    ***		4 6-i-1		3 i*2+1
//   *****		3 6-i-1		5 i*2+1
//  *******		2 6-i-1		7 i*2+1
// *********	1 6-i-1		9 i*2+1
//***********	0 6-i-1		11 i*2+1
func main() {

	//行数
	l := 6

	//整体执行次数
	for i := 0; i < l; i++ {
		//控制空格个数
		for j := 0; j < l-i-1; j++ {
			fmt.Print(" ")
		}
		//控制星星个数
		for k := 0; k < i*2+1; k++ {
			fmt.Print("*")
		}
		fmt.Println()
	}

}
