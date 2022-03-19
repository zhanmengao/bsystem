package main

import (
	"bufio"
	"fmt"
	"os"
)
/*
bufio包实现了有缓冲的I/O。它包装一个io.Reader或io.Writer接口对象，创建另一个也实现了该接口，且同时还提供了缓冲和一些文本I/O的帮助函数的对象。

简单的说就是bufio会把文件内容读取到缓存中（内存），然后再取读取需要的内容的时候，直接在缓存中读取，避免文件的i/o操作。

同样，通过bufio写入内容，也是先写入到缓存中（内存），然后由缓存写入到文件。避免多次小内容的写入操作I/O。
 */
func main(){
	counts := make(map[string]int)				//map<string,int>

	input := bufio.NewScanner(os.Stdin)
	//while
	for input.Scan(){
		counts[input.Text()]++			//insert 2 map

		for line,n:=range counts{
			if n>1 {
				fmt.Printf("%d \t %s \n",n,line);
			}
		}
	}
}