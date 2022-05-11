package RoutinuePool

import (
	"log"
	"sync"
	"sync/atomic"
	"time"
)

type cb func(interface{}) error
type blockQueue chan *threadPoolTask
type threadPoolTask struct {
	function cb
	argv     interface{}
}
type ThreadPool struct {
	queue   blockQueue
	wg      sync.WaitGroup
	command chan ThreadCommand

	busy      int32
	total     int32
	taskCount int32
	taskMax   int32
}

type ThreadCommand int32

const (
	ThreadCommandKill = iota
)

const ThreadIncrCount = 5

func NewThreadPool(nThreadCount, nTaskMax int32) *ThreadPool {
	pool := &ThreadPool{
		queue:   make(blockQueue, nTaskMax),
		command: make(chan ThreadCommand, 100),
		total:   nThreadCount,
		taskMax: nTaskMax,
	}
	go pool.adjustThread()
	for i := 0; int32(i) < nThreadCount; i++ {
		go pool.worker()
	}
	return pool
}

func (pool *ThreadPool) adjustThread() {
	for {
		if pool.isBusy() {
			for i := 0; i < ThreadIncrCount; i++ {
				go pool.worker()
			}
		} else if pool.isFree() {
			for i := 0; i < ThreadIncrCount; i++ {
				pool.command <- ThreadCommandKill
			}
		}
		time.Sleep(time.Duration(1) * time.Second)
	}
}
func (pool *ThreadPool) worker() {
	for {
		select {
		case task := <-pool.queue:
			{
				pool.call(task)
			}
		case command := <-pool.command:
			{
				switch command {
				case ThreadCommandKill:
					pool.workerKill()
					return
				}
			}
		}
	}
}

func (pool *ThreadPool) PushTask(function cb, argv interface{}) {
	pool.queue <- &threadPoolTask{
		argv:     argv,
		function: function,
	}
	atomic.AddInt32(&pool.taskCount, 1)
}

func (pool *ThreadPool) call(task *threadPoolTask) {
	pool.IncrBusyThreadCount()
	defer pool.DimBusyThreadCount()
	err := task.function(task.argv)
	if err != nil {
		log.Fatal(err)
	}
}

func (pool *ThreadPool) IncrBusyThreadCount() {
	pool.wg.Add(1)
	atomic.AddInt32(&pool.busy, 1)
	atomic.AddInt32(&pool.taskCount, -1)
}
func (pool *ThreadPool) DimBusyThreadCount() {
	pool.wg.Done()
	atomic.AddInt32(&pool.busy, -1)
}

func (pool *ThreadPool) isBusy() bool {
	return atomic.LoadInt32(&pool.total) <= atomic.LoadInt32(&pool.busy) ||
		atomic.LoadInt32(&pool.taskCount) >= atomic.LoadInt32(&pool.taskMax)
}
func (pool *ThreadPool) isFree() bool {
	return (atomic.LoadInt32(&pool.total)-atomic.LoadInt32(&pool.busy)) >= 5 ||
		atomic.LoadInt32(&pool.taskCount) == 0
}

func (pool *ThreadPool) workerInc(inc int32) {
	atomic.AddInt32(&pool.total, inc)
}
func (pool *ThreadPool) workerKill() {
	atomic.AddInt32(&pool.total, -1)
}
func (pool *ThreadPool) Destroy() {
	for i := 0; int32(i) < atomic.LoadInt32(&pool.total); i++ {
		pool.command <- ThreadCommandKill
	}
	pool.wg.Wait()
}
