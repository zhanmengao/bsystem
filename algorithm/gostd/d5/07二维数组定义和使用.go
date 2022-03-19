package main

import "fmt"

func main0701() {

	//var arr [10]int 一维数组

	var arr [2][3]int

	arr[0][1] = 123
	arr[1][2] = 234

	//fmt.Println(arr)

	//len(二维数组名)一个二维数组有几行
	//fmt.Println(len(arr))
	//len(二维数组名[下标])一个二维数组有几列
	//fmt.Println(len(arr[0]))

	//外层控制行 内层控制列
	//for i := 0; i < len(arr); i++ {
	//	for j := 0; j < len(arr[0]); j++ {
	//		fmt.Println(arr[i][j])
	//	}
	//}

	for _, v := range arr {
		//fmt.Println(i,v)
		//fmt.Printf("%T\n",v)

		for _, data := range v {
			fmt.Println(data)
		}
	}

}
func main() {
	//二维数组定义方式
	//var arr [2][3]int =[2][3]int{{1,2,3},{2,3,4}}
	//var arr [2][3]int = [2][3]int{{1, 1}, {1}}
	// var arr[2][3]int=[2][3]int{1:{1:1}}
	arr:=[2][3]int{{1,2,3},{2,3}}
	fmt.Println(arr)
}
