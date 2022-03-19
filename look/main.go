package main

import (
	"fmt"
	"pc/pubsub"
	"strings"
	"time"
)

func psMain(){
	p:=pubsub.NewPublisher(100*time.Microsecond,10)
	defer p.Close()

	all:=p.Subscribe()
	golang:=p.SubscribeTopic(func(v interface{})bool{
		if s,ok:= v.(string);ok{
			return strings.Contains(s,"golang")
		}
		return false
	})

	p.Publish("hello world")
	p.Publish("hello golang")

	go func(){
		for msg:= range all{
			fmt.Println("all : ",msg)
		}
	}()

	go func(){
		for msg:= range golang{
			fmt.Println("golang : ",msg)
		}
	}()
	time.Sleep(3*time.Second)
}
func main(){
	//sem.ConsumerMain()
	psMain()
}