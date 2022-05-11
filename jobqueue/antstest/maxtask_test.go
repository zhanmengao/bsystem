package main

import (
	"fmt"
	"github.com/panjf2000/ants/v2"
	"sync"
	"testing"
)

//ants池设置容量之后，如果所有的 goroutine 都在处理任务。 这时提交的任务默认会进入等待队列，WithMaxBlockingTasks(maxBlockingTasks int)可以设置等待队列的最大长度。
//超过这个长度，提交任务直接返回错误
func TestMaxTask(t *testing.T) {
	p, _ := ants.NewPool(4, ants.WithMaxBlockingTasks(2))
	defer p.Release()

	var wg sync.WaitGroup
	wg.Add(8)
	for i := 1; i <= 8; i++ {
		go func(i int) {
			err := p.Submit(wrapper(i, &wg))
			if err != nil {
				fmt.Printf("task:%d err:%v\n", i, err)
				wg.Done()
			}
		}(i)
	}

	wg.Wait()
}

//提交任务必须并行进行。如果是串行提交，第 5 个任务提交时由于池中没有空闲的 goroutine 处理该任务，Submit()方法会被阻塞，后续任务就都不能提交了。也就达不到验证的目的了
//由于任务可能提交失败，失败的任务不会实际执行，所以实际上wg.Done()次数会小于 8。因而在err != nil分支中我们需要调用一次wg.Done()。否则wg.Wait()会永远阻塞
//为了避免任务执行过快，空出了 goroutine，观察不到现象，每个任务中我使用time.Sleep(1 * time.Second)休眠 1s
//由于 goroutine 之间的执行顺序未显式同步，故每次执行的顺序不确定
