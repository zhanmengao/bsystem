package antsqueue

import (
	"errors"
	"github.com/panjf2000/ants/v2"
	"sync"
)

type AntsQueue struct {
	isOpen bool
	queue  *ants.PoolWithFunc
	job    chan func() //放func的channel
	close  chan struct{}
	wg     sync.WaitGroup
	//用户的请求列表
}

func NewAntsQueue(sz int) *AntsQueue {
	ret := &AntsQueue{
		job:    make(chan func(), sz),
		close:  make(chan struct{}, 0),
		isOpen: true,
	}
	ret.queue, _ = ants.NewPoolWithFunc(sz, ret.do, ants.WithPanicHandler(panicHandler))
	return ret
}

func (q *AntsQueue) Run() {
	defer q.queue.Release()
	defer close(q.job)
	defer close(q.close)
	for {
		select {
		case f := <-q.job:
			q.queue.Invoke(f)
		case <-q.close:
			//取出所有消息，处理完return
			q.clear()
			q.wg.Wait()
		}
	}
}
func (q *AntsQueue) PushJob(f func()) error {
	if !q.isOpen {
		return errors.New("is close")
	}
	q.wg.Add(1)
	q.job <- f
	return nil
}

func (q *AntsQueue) Close() {
	q.isOpen = false
	if q.close != nil {
		q.close <- struct{}{}
	}
}
func (q *AntsQueue) Wait() {
	q.wg.Wait()
}

func (q *AntsQueue) do(iFunc interface{}) {
	f := iFunc.(func())
	f()
	q.wg.Done()
}
func (q *AntsQueue) clear() {
	for {
		select {
		case f := <-q.job:
			q.queue.Invoke(f)
		default:
			return
		}
	}
}
