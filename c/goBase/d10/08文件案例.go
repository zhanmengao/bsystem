package main

import (
	"os"
	"fmt"
	"io"
)

func main() {
	//读取文件
	fp1, err1 := os.Open("D:/0824基础班/01_Go语言基础第07天（指针）/03视频/03结构体和函数.avi")
	//写入文件
	fp2, err2 := os.Create("D:/test.avi")

	if err1 != nil || err2 != nil {
		fmt.Println("操作文件失败")
		return
	}
	//关闭文件
	defer fp1.Close()
	defer fp2.Close()

	//拷贝文件
	//通过read块进行文件读取
	//通过write进行写入
	buf := make([]byte, 1024*1024*10) //1KB 1MB  1KB-16MB
	for {
		//将读取的字符写入到新文件中
		n, err := fp1.Read(buf)
		if err == io.EOF {
			break
		}
		fp2.Write(buf[:n])
	}
}
