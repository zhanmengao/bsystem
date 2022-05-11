package main

import (
	"fmt"
	"github.com/panjf2000/ants/v2"
	"math/rand"
	"sync"
	"testing"
)

func taskFuncWrapper(nums []int, i int, sum *int, wg *sync.WaitGroup) func() {
	return func() {
		for _, num := range nums[i*DataPerTask : (i+1)*DataPerTask] {
			*sum += num
		}

		fmt.Printf("task:%d sum:%d\n", i+1, *sum)
		wg.Done()
	}
}

//ants支持将一个不接受任何参数的函数作为任务提交给 goroutine 运行。
func TestNew(t *testing.T) {

	//提交函数作为任务的 goroutine 池使用ants.NewPool()创建，它只接受一个参数表示池子的容量。
	//调用池子对象的Submit()方法来提交任务，将一个不接受任何参数的函数传入。
	p, _ := ants.NewPool(10)
	defer p.Release()

	nums := make([]int, DataSize, DataSize)
	for i := range nums {
		nums[i] = rand.Intn(1000)
	}

	var wg sync.WaitGroup
	wg.Add(DataSize / DataPerTask)
	partSums := make([]int, DataSize/DataPerTask, DataSize/DataPerTask)
	//由于不接受参数，我们提交的函数要么不需要外部数据，只需要处理自身逻辑，否则就必须用某种方式将需要的数据传递进去，例如闭包
	for i := 0; i < DataSize/DataPerTask; i++ {
		p.Submit(taskFuncWrapper(nums, i, &partSums[i], &wg))
	}
	wg.Wait()

	//汇总结果，验证：
	var sum int
	for _, partSum := range partSums {
		sum += partSum
	}

	var expect int
	for _, num := range nums {
		expect += num
	}
	fmt.Printf("running goroutines: %d\n", ants.Running())
	fmt.Printf("finish all tasks, result is %d expect is %d\n", sum, expect)
}
