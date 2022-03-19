// 注释  行注释
/*
块注释
可以注释多行
注释中的内容不参与程序编译
 */
 //文件所属的包 在go语言中 主函数所在的包一定是main
package main

import "fmt"

//导入系统包 标准输入输出包

//func 函数格式 main 函数名 主函数  程序有且只有一个主函数  无论程序中有多少函数 都会从main进入
//() 函数参数列表 {}函数体 代码体 程序体
func main(){
	//fmt包  Println 打印并且换行 ""引起来的称为字符串
	fmt.Println("hello world")
	fmt.Println("传智播客")
}


//注释快捷键是ctrl + /