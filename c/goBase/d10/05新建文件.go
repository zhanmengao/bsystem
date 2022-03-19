package main

import (
	"os"
	"fmt"
)

func main() {

	//os.Create(文件名)文件名 可以写绝对路径 也可以写相对路径
	//返回值为 文件指针 错误信息
	//如果文件不存在会创建新文件 如果文件存在会清空源文件内容
	fp, err := os.Create("D:/a.txt")

	if err != nil {
		//文件创建失败
		/*
		1、路径不存在
		2、文件权限
		3、程序打开文件上限 65535
		 */
		fmt.Println("文件创建失败")
		return
	}

	//延迟调用关闭文件
	//defer fp.Close()

	//文件创建成功 可以操作文件

	//关闭文件
	//如果打开文件不关闭 会造成内存浪费 程序打开文件上限
	fp.Close()

}
