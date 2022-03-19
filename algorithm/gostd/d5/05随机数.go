package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {

	//1、导入头文件  math/rand  time
	//2、随机数种子
	//3、创建随机数

	//创建随机数种子 进行数据混淆
	rand.Seed(time.Now().UnixNano())

	//伪随机数 使用的1970.1.1.0.0.0
	//fmt.Println(rand.Intn(10))//取模10   得到0-9
	for i := 0; i < 10; i++ {
		fmt.Println(rand.Intn(10))
	}
}
