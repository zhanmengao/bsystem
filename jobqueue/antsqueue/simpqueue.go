package antsqueue

import (
	"github.com/panjf2000/ants/v2"
)

type AntsSimpleQueue struct {
	queue *ants.Pool
	job   chan func() //放func的channel
	close chan struct{}
	//用户的请求列表
}

func NewAntsSimpleQueue(sz int) *AntsSimpleQueue {
	ret := &AntsSimpleQueue{
		job:   make(chan func(), sz),
		close: make(chan struct{}, 0),
	}
	ret.queue, _ = ants.NewPool(sz, ants.WithPanicHandler(panicHandler))
	return ret
}

func (q *AntsSimpleQueue) Run() {
	defer q.queue.Release()
	defer close(q.job)
	defer close(q.close)
	for {
		select {
		case f := <-q.job:
			q.queue.Submit(f)
		case <-q.close:
			//取出所有消息，处理完return
			q.clear()
		}
	}
}
func (q *AntsSimpleQueue) PushJob(f func()) {
	if q.job != nil {
		q.job <- f
	}
}

func (q *AntsSimpleQueue) Close() {
	if q.close != nil {
		q.close <- struct{}{}
	}
}

func (q *AntsSimpleQueue) clear() {
	for {
		select {
		case f := <-q.job:
			q.queue.Submit(f)
		default:
			return
		}
	}
}
