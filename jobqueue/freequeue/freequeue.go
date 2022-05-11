package freequeue

import (
	"strconv"
	"sync"
)

type FreeQueue struct {
	worker []*tWorker
	cap    int32

	closeCh chan struct{}
	wg      sync.WaitGroup
}

func NewFreeQueue(sz int32) *FreeQueue {
	fq := &FreeQueue{
		worker:  make([]*tWorker, sz),
		cap:     sz,
		closeCh: make(chan struct{}, sz),
	}
	for i, _ := range fq.worker {
		fq.worker[i] = NewWorker(sz)
		go fq.worker[i].run(fq.closeCh, &fq.wg)
	}
	return fq
}

func (fq *FreeQueue) Push(key string, f func()) {
	fq.worker[fq.getHashKey(key)].push(&tJob{
		f:   f,
		key: key,
	})
}

func (fq *FreeQueue) getHashKey(uid string) int32 {
	uid64, _ := strconv.ParseInt(uid, 10, 64)
	return int32(uid64 % int64(fq.cap))
}
