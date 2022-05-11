package main

import "fmt"

const (
	MAX_QUEUE = 256
	MAX_WORKER = 32
	MAX_WORKER_POOL_SIZE = 5
)

var JobQueue chan string

// Worker 用来管理执行管道中的任务
type Worker struct {
	WorkerPool chan chan string
	JobChannel chan string
	quit chan bool
}

func NewWorker(workerPool chan chan string) *Worker {
	return &Worker{
		WorkerPool:workerPool,
		JobChannel:make(chan string),
		quit:    make(chan bool),
	}
}

// Start 开启当前的 worker，循环上传 channel 中的 job；并同时监听 quit 退出标识位，判断是否关闭该 channel
func (w *Worker) Start() {
	go func() {
		for {
			w.WorkerPool <- w.JobChannel
			select {
			case job := <-w.JobChannel:
				fmt.Println(job)
			case <-w.quit://收到停止信号，退出
				return
			}
		}
	}()
}

// Stop 关闭该 channel，这里是将 channel 标识位设置为关闭，实际在 worker 执行中关闭
func (w *Worker) Stop() {
	go func() {
		w.quit <- true
	}()
}

type Dispatcher struct {
	WorkerPool chan chan string
	quit       chan bool
}

func NewDispatcher(maxWorkers int) *Dispatcher {
	pool := make(chan chan string, maxWorkers)
	return &Dispatcher{WorkerPool: pool}
}

func (d *Dispatcher) dispatcher() {
	for {
		select {
		case job := <-JobQueue:
			go func(job string) {
				jobChannel := <-d.WorkerPool
				jobChannel <- job
			}(job)
		case <-d.quit:
			return
		}
	}
}

func (d *Dispatcher) Run() {
	for i := 0; i < MAX_WORKER_POOL_SIZE; i++ {
		worker := NewWorker(d.WorkerPool)
		worker.Start()
	}
	go d.dispatcher()
}

