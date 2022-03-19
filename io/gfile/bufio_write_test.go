package gio

import (
	"bufio"
	"fmt"
	"os"
	"testing"
)

//此种方法可以在文件内容末尾添加新内容。
func TestBufioWrite(t *testing.T) {
	var str = "测试1\n测试2\n"
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
	if err1 != nil {
		panic(err1)
	}
	w := bufio.NewWriter(f) //创建新的 Writer 对象
	n, _ := w.WriteString(str)
	fmt.Printf("写入 %d 个字节n", n)
	w.Flush()
}

