package main

import (
	"fmt"
	"github.com/panjf2000/ants/v2"
	"sync"
	"testing"
)

//ants池默认是阻塞的，我们可以使用WithNonblocking(nonblocking bool)设置其为非阻塞。非阻塞的ants池中，在所有 goroutine 都在处理任务时，提交新任务会直接返回错误：
func TestBlocking(t *testing.T) {
	//ants池容量设置为 2。连续提交 3 个任务，期望结果前两个任务正常执行，第 3 个任务提交时返回错误：
	p, _ := ants.NewPool(2, ants.WithNonblocking(true))
	defer p.Release()

	var wg sync.WaitGroup
	wg.Add(3)
	for i := 1; i <= 3; i++ {
		err := p.Submit(wrapper(i, &wg))
		if err != nil {
			fmt.Printf("task:%d err:%v\n", i, err)
			wg.Done()
		}
	}

	wg.Wait()
}
