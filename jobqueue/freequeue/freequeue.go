package freequeue

import (
	"errors"
	"strconv"
)

type FreeQueue struct {
	worker  []*tWorker
	cap     int32
	isOpen  bool
	closeCh chan struct{}
}

func NewFreeQueue(sz int32) *FreeQueue {
	fq := &FreeQueue{
		worker:  make([]*tWorker, sz),
		cap:     sz,
		closeCh: make(chan struct{}, sz),
		isOpen:  true,
	}
	for i, _ := range fq.worker {
		fq.worker[i] = NewWorker(sz)
	}
	return fq
}

func (fq *FreeQueue) Run() {
	for i, _ := range fq.worker {
		go fq.worker[i].run(fq.closeCh)
	}
}

func (fq *FreeQueue) PushJob(key string, f func()) error {
	if !fq.isOpen {
		return errors.New("is close")
	}
	fq.worker[fq.getHashKey(key)].push(&tJob{
		f:   f,
		key: key,
	})
	return nil
}

func (fq *FreeQueue) Close() {
	fq.isOpen = false
	for _, _ = range fq.worker {
		fq.closeCh <- struct{}{}
	}
}

func (fq *FreeQueue) Wait() {
	for _, w := range fq.worker {
		w.Wait()
	}
}

func (fq *FreeQueue) getHashKey(uid string) int32 {
	uid64, _ := strconv.ParseInt(uid, 10, 64)
	return int32(uid64 % int64(fq.cap))
}
