package antsqueue

import (
	"github.com/panjf2000/ants/v2"
	"sync"
)

type AntsJobQueue struct {
	queue *ants.PoolWithFunc
	job   chan *userRequestList //放func的channel
	close chan struct{}
	wg    sync.WaitGroup
	//用户的请求列表
	userRequest sync.Map
}

func NewAntsJobQueue(sz int) *AntsJobQueue {
	ret := &AntsJobQueue{
		job:   make(chan *userRequestList, sz),
		close: make(chan struct{}, 0),
	}
	ret.queue, _ = ants.NewPoolWithFunc(sz, ret.do)
	return ret
}

func (q *AntsJobQueue) do(iUser interface{}) {
	u := iUser.(*userRequestList)
	for {
		select {
		case f := <-u.request:
			f()
		default:
			return
		}
	}
}

func (q *AntsJobQueue) Run() {
	defer q.queue.Release()
	for {
		select {
		case u := <-q.job:
			q.queue.Invoke(u)
		case <-q.close:
			//取出所有消息，处理完return
		}
	}
}

func (q *AntsJobQueue) clear() {
	for {
		select {
		case u := <-q.job:
			q.queue.Invoke(u)
		default:
			close(q.job)
			return
		}
	}
}
