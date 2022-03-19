package RoutinuePool

import (
	"fmt"
	"testing"
)


func TestThreadPool(t *testing.T) {
	pool := NewThreadPool(100,500)
	for i:=0;i<10000;i++{
		pool.PushTask(func(argv interface{})error{
			fmt.Println(argv)
			return nil
		},"hello world")
	}
	pool.Destroy()
}
