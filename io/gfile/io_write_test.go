package gio

import (
	"fmt"
	"io"
	"os"
	"testing"
)

func checkFileIsExist(filename string) bool {
	if _, err := os.Stat(filename); os.IsNotExist(err) {
		return false
	}
	return true
}

//此种方法可以在文件内容末尾添加新内容。
func TestWriteString(t *testing.T) {
	var writeString = "测试1\n测试2\n"
	var filename = "./test.txt"
	var f *os.File
	var err1 error
	if checkFileIsExist(filename) { //如果文件存在
		f, err1 = os.OpenFile(filename, os.O_APPEND, 0666) //打开文件
		fmt.Println("文件存在")
	} else {
		f, err1 = os.Create(filename) //创建文件
		fmt.Println("文件不存在")
	}
	defer f.Close()
	n, err1 := io.WriteString(f, writeString) //写入文件(字符串)
	if err1 != nil {
		panic(err1)
	}
	fmt.Printf("写入 %d 个字节n", n)
}