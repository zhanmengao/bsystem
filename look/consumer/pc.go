package sem

import (
	"fmt"
	"time"
)

func producer(factor int,out chan<- int){
	for i:=0;;i++{
		out<- i*factor
	}
}

func consumer(in <-chan int){
	for v:= range in{
		fmt.Println(v)
	}
}

func ConsumerMain(){
	ch := make(chan int,64)			//成果队列

	go producer(3,ch)
	go producer(5,ch)
	go consumer(ch)

	time.Sleep(time.Duration(5)*time.Second)
}
