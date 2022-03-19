package main

import (
	"os"
	"fmt"
	"io"
	"bufio"
)

func main0701() {
	//只读方式打开文件
	//fp, err := os.Open("D:/a.txt")
	fp, err := os.Open("D:/dict.txt")
	if err != nil {
		fmt.Println("打开文件失败", err)
		return
	}
	defer fp.Close()
	buf := make([]byte, 10)
	//块读取
	//n, _ := fp.Read(buf)
	//
	//fmt.Print(string(buf[:n]))
	//n, _ = fp.Read(buf)
	//fmt.Print(string(buf[:n]))
	//n, _ = fp.Read(buf)
	//fmt.Print(string(buf[:n]))

	for {
		n, err := fp.Read(buf)
		//io.EOF 表示的的是文件的结尾
		//放读取到文件末尾 返回值为errors.New("EOF")
		if err == io.EOF {
			break
		}
		fmt.Print(string(buf[:n]))
	}
}
func main() {

	fp, err := os.OpenFile("D:/a.txt", os.O_RDONLY, 6)
	if err != nil {
		fmt.Println("文件打开失败")
		return
	}
	defer fp.Close()
	//创建文件缓冲区
	r := bufio.NewReader(fp)
	//行读取  截取的标志位'\n'  可以使用ASCII中任意字符
	//slice, _ := r.ReadBytes('\n')
	//
	//fmt.Print(string(slice))
	//
	//slice,_=r.ReadBytes('\n')
	//fmt.Print(string(slice))

	for {
		str, err := r.ReadString('\n')

		fmt.Print(str)
		//先打印在判断
		if err == io.EOF {
			break
		}
	}
}
