package main

import "fmt"

func main0801() {
	var i interface{}
	i = 10.234
	//value,ok:=map[key]
	//值,值的判断:=接口变量.(数据类型)
	value, ok := i.(int)
	if ok {
		fmt.Println("整型数据：", value+10)
	} else {
		fmt.Println("错误")
	}

}
func demo() {
	fmt.Println("demo hello world")
}
func main() {
	var i []interface{}

	i = append(i, 10, 3.14, "传智播客", demo,123,test)

	for _, v := range i {
		//fmt.Println(idx,v)

		if data, ok := v.(int); ok {
			fmt.Println("整型数据：", data)
		} else if data, ok := v.(float64); ok {
			fmt.Println("浮点型数据：", data)
		} else if data, ok := v.(string); ok {
			fmt.Println("字符串数据：", data)
		} else if data, ok := v.(func()); ok {
			//函数调用
			data()
		}
	}

}
