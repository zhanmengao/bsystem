package main

import (
	"fmt"
	"testing"
	"time"
)

/*
使用`select`语句能提高代码的可读性。

- 可处理一个或多个channel的发送/接收操作。
- 如果多个`case`同时满足，`select`会随机选择一个。
- 对于没有`case`的`select{}`会一直等待，可用于阻塞main函数。

说到这里，要提一下，之前介绍说`channel`的零值为nil，咦？对一个`nil`的`channel`发送和接收操作会永远阻塞！针对这个特性，
在`select`语句中操作`nil`的`channel`是不是永远也不会被`select`到了呢！
基于此，我们可以通过`nil`去激活或者禁用`case`，来完成处理输入输出事件超时和取消的逻辑。
*/
func Test_Select(t *testing.T) {
	{
		ch := make(chan int, 1)
		for i := 0; i < 10; i++ {
			select {
			case x := <-ch:
				fmt.Println(time.Now().Format("2006-01-02 15:04:05"), " [接收来自通道中的值]:", x)
			case ch <- i:
				fmt.Println(time.Now().Format("2006-01-02 15:04:05"), " [向通道中发送msg]:", i)
			}
		}
	}
	fmt.Println()

}

func Test_Default(t *testing.T) {
	ch := make(chan int, 1)

	ch <- 1

	select {
	case msg := <-ch:
		fmt.Println(msg)
	default:
		fmt.Println("default")
	}
}

//default可以判断chan是否已经满了
func Test_Full(t *testing.T){
	ch := make(chan int)
	select {
	case ch<-1:
		fmt.Println("ch")
	default:
		fmt.Println("default")
	}
}
