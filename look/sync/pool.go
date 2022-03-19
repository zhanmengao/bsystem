package main

import (
	"fmt"
	"sync"
	"time"
)
var pool sync.Pool
func NewConnect()interface{}{
	return make(chan int64)
}
func main(){
	pool.New = NewConnect
	conn := pool.Get()
	ch := conn.(chan int64)
	ch <- time.Now().Unix()
	go func(ch chan int64){
		tm := <-ch
		fmt.Printf("tm = %d \n",tm)
		pool.Put(ch)
	}(ch)
}