package freequeue

import (
	"container/list"
	"sync"
	"sync/atomic"
)

type tJob struct {
	f   func()
	key string
}

type tWorker struct {
	ch    chan *tJob
	chLen int32

	fullCh   chan struct{} //队列满时，做一个标记，然后push函数退出
	fullFlag int32         //队列清空时为0，队列有东西就是>0
	fullList *list.List
	fullLock sync.Mutex
	wg       sync.WaitGroup
}

func NewWorker(cap int32) *tWorker {
	return &tWorker{
		ch:       make(chan *tJob, cap),
		fullList: list.New(),
		fullCh:   make(chan struct{}, 1),
	}

}

func (worker *tWorker) push(job *tJob) {
	worker.wg.Add(1)
	if worker.fullFlag == 0 && worker.casAdd(job) {
	} else {
		//队列满，加到fullList
		worker.fullLock.Lock()
		worker.fullList.PushBack(job)
		worker.fullLock.Unlock()
		if atomic.CompareAndSwapInt32(&worker.fullFlag, 0, 1) {
			worker.fullCh <- struct{}{}
		}
	}
}

func (worker *tWorker) doFullList() {
	worker.fullLock.Lock()
	defer worker.fullLock.Unlock()
	for item := worker.fullList.Front(); ; {
		current := item
		//遍历完了，退出
		if current == nil {
			worker.fullFlag = 0
			break
		}
		item = item.Next()
		if worker.casAdd(current.Value.(*tJob)) { //ch还能add，增加到队尾
			worker.fullList.Remove(current)
		} else {
			worker.fullCh <- struct{}{} //ch已经被塞满了，退出，将struct塞回去
			break
		}
	}
}

func (worker *tWorker) run(closeCh <-chan struct{}) {
	for {
		if worker.isFull() {
			select {
			case w := <-worker.ch:
				worker.casDel()
				w.f()
				worker.wg.Done()
			default:
			}
		} else {
			select {
			case w := <-worker.ch:
				worker.casDel()
				w.f()
				worker.wg.Done()
			case <-worker.fullCh:
				//遍历list，尽可能取出请求放到ch
				worker.doFullList()
			case <-closeCh:
				worker.clear()
				worker.Wait()
				return
			}
		}
	}
}

func (worker *tWorker) Wait() {
	worker.wg.Wait()
}

func (worker *tWorker) clear() {
	defer close(worker.ch)
	for {
		select {
		case w := <-worker.ch:
			worker.casDel()
			w.f()
			worker.wg.Done()
		case <-worker.fullCh:
			//遍历list，尽可能取出请求放到ch
			worker.doFullList()
		}
	}
}
func (worker *tWorker) isFull() bool {
	if len(worker.ch) >= cap(worker.ch) {
		return true
	}
	return false
}

func (worker *tWorker) casAdd(job *tJob) bool {
	for {
		cl := worker.chLen
		//队列满，直接return
		if worker.isFull() {
			return false
		}
		//只有我缓存下来的len和实际len一样时，才发生add。此时队列不满
		if atomic.CompareAndSwapInt32(&worker.chLen, cl, cl+1) {
			worker.ch <- job
			return true
		}
		continue
	}
}

func (worker *tWorker) casDel() {
	atomic.AddInt32(&worker.chLen, -1)
}
