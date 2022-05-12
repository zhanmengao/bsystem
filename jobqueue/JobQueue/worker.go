package jobqueue

import "sync"

type orderWorker struct {
	ch chan *orderJob
}

func newOrderWorker() *orderWorker {
	w := &orderWorker{
		ch: make(chan *orderJob, 100),
	}
	return w
}

func (w *orderWorker) work(doneCh chan<- *orderJobDone, closeCh <-chan bool, wg *sync.WaitGroup) {
	for {
		select {
		case j := <-w.ch:
			j.fn()
			//将完成的调用放到队列
			doneCh <- &orderJobDone{
				key: j.key,
				ch:  w.ch,
			}
		case <-closeCh:
			w.clear(doneCh)
			wg.Done()
			return
		}
	}
}

func (w *orderWorker) clear(doneCh chan<- *orderJobDone) {
	for {
		select {
		case j := <-w.ch:
			j.fn()
			//将完成的调用放到队列
			doneCh <- &orderJobDone{
				key: j.key,
				ch:  w.ch,
			}
		default:
			return
		}
	}
}
