package main

import (
	"fmt"
	"testing"
)

func Test_Channel(t *testing.T) {
	ch := make(chan int, 1) //改为 ch := make(chan int, 1) 就好了

	ch <- 1

	fmt.Println("success")
}


func Test_Channel2(t *testing.T) {
	ch := make(chan string)
	go setData(ch)
	fmt.Println(<-ch)
	fmt.Println(<-ch)
	fmt.Println(<-ch)
	fmt.Println(<-ch)
	fmt.Println(<-ch)
}
func setData(ch  chan  string){
	ch <- "test"
	ch <- "hello wolrd"
	ch <- "123"
	ch <- "456"
	ch <- "789"
}

func TestChannel3(t *testing.T){
	channel := make(chan string, MAX_QUEUE)

	go func(msg string) {
		channel <- msg //向信道中存消息
	}("ping")

	msg := <- channel //从信道中去消息
	fmt.Println(msg)
}