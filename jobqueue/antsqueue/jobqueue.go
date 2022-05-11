package antsqueue

import (
	"github.com/panjf2000/ants/v2"
	"sync"
)

type AntsQueue struct {
	queue *ants.Pool
	job   chan func() //放func的channel
	close chan struct{}
	wg    sync.WaitGroup
	//用户的请求列表
}

func NewAntsQueue(sz int) *AntsQueue {
	ret := &AntsQueue{
		job:   make(chan func(), sz),
		close: make(chan struct{}, 0),
	}
	ret.queue, _ = ants.NewPool(sz)
	return ret
}

func (q *AntsQueue) Run() {
	defer q.queue.Release()
	for {
		select {
		case f := <-q.job:
			q.queue.Submit(f)
		case <-q.close:
			//取出所有消息，处理完return
			q.clear()
			q.wg.Wait()
		}
	}
}

func (q *AntsQueue) clear() {
	for {
		select {
		case f := <-q.job:
			q.queue.Submit(f)
		default:
			close(q.job)
			return
		}
	}
}
