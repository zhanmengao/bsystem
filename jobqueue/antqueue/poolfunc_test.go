package main

import (
	"fmt"
	"github.com/panjf2000/ants/v2"
	"math/rand"
	"sync"
	"testing"
)

const (
	DataSize    = 10000
	DataPerTask = 100
)

//上面调用了ants.NewPoolWithFunc()创建了一个 goroutine 池。第一个参数是池容量，即池中最多有 10 个 goroutine。 第二个参数为每次执行任务的函数。
//当我们调用p.Invoke(data)的时候，ants池会在其管理的 goroutine 中找出一个空闲的，让它执行函数taskFunc，并将data作为参数。
func TestPoolFunc(t *testing.T) {
	p, _ := ants.NewPoolWithFunc(10, taskFunc)
	defer p.Release()

	//模拟数据，做数据切分，生成任务，交给 ants 处理
	nums := make([]int, DataSize, DataSize)
	for i := range nums {
		nums[i] = rand.Intn(1000)
	}

	//随机生成 10000 个整数，将这些整数分为 100 份，每份 100 个，生成Task结构，调用p.Invoke(task)处理。
	//wg.Wait()等待处理完成，然后输出ants正在运行的 goroutine 数量，这时应该是 0
	var wg sync.WaitGroup
	wg.Add(DataSize / DataPerTask)
	tasks := make([]*Task, 0, DataSize/DataPerTask)
	for i := 0; i < DataSize/DataPerTask; i++ {
		task := &Task{
			index: i + 1,
			nums:  nums[i*DataPerTask : (i+1)*DataPerTask],
			wg:    &wg,
		}

		tasks = append(tasks, task)
		p.Invoke(task)
	}

	wg.Wait()
	fmt.Printf("running goroutines: %d\n", ants.Running())

	//将结果汇总，并验证一下结果，与直接相加得到的结果做一个比较
	//goroutine 池中任务的执行顺序是随机的，与提交任务的先后没有关系
	var sum int
	for _, task := range tasks {
		sum += task.sum
	}

	var expect int
	for _, num := range nums {
		expect += num
	}

	fmt.Printf("finish all tasks, result is %d expect:%d\n", sum, expect)
}
