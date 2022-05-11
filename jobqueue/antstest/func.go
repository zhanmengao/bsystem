package main

import (
	"fmt"
	"sync"
	"time"
)

func taskFunc(data interface{}) {
	task := data.(*Task)
	task.Do()
	fmt.Printf("task:%d sum:%d\n", task.index, task.sum)
}

func wrapper(i int, wg *sync.WaitGroup) func() {
	return func() {
		fmt.Printf("hello from task:%d\n", i)
		time.Sleep(1 * time.Second)
		wg.Done()
	}
}

func wrapperPanic(i int, wg *sync.WaitGroup) func() {
	//我们让偶数个任务触发panic。提交两个任务，第二个任务一定会触发panic。触发panic之后，我们还可以继续提交任务 3、5。注意这里没有 4，提交任务 4 还是会触发panic。
	return func() {
		fmt.Printf("hello from task:%d\n", i)
		if i%2 == 0 {
			panic(fmt.Sprintf("panic from task:%d", i))
		}
		wg.Done()
	}
}
