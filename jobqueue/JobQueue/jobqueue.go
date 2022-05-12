package jobqueue

import (
	"container/list"
	"errors"
	"sync"
	"sync/atomic"
)

type orderJob struct {
	key string
	fn  func()
}

type orderJobDone struct {
	key string
	ch  chan *orderJob
}

// OrderCallback callback function
type OrderCallback func(k string, fn func())

const (
	orderqStatRunning = iota
	orderqStatClosed
	orderqStatGracefulClosed
)

// OrderJobQueue order queue of job
type OrderJobQueue struct {
	size    int
	ch      chan *orderJob  //接收任务
	bList   *list.List      // 候补队列
	lockMap map[string]bool // 锁map

	closed     int32
	closeWg    sync.WaitGroup
	closeCh    chan bool
	doneCh     chan *orderJobDone // job done channel
	idleWorker []chan *orderJob   // 空闲 worker的工作 channel

}

// NewOrderJobQueue create an OrderJobQueue
func NewOrderJobQueue(size int) (q *OrderJobQueue, err error) {
	q = &OrderJobQueue{
		size:       size,
		ch:         make(chan *orderJob, size),
		bList:      list.New(),
		lockMap:    make(map[string]bool),
		doneCh:     make(chan *orderJobDone, size),  //已经完成的调用
		idleWorker: make([]chan *orderJob, 0, size), //空闲工作者队列

		closed:  orderqStatRunning,
		closeWg: sync.WaitGroup{},
		closeCh: make(chan bool, size),
	}
	return
}

func (q *OrderJobQueue) Run() {
	for i := 0; i < q.size; i++ {
		w := newOrderWorker()
		q.idleWorker = append(q.idleWorker, w.ch)
		go w.work(q.doneCh, q.closeCh, &q.closeWg)
	}
	q.loop()
}

// queue errors
var (
	ErrOrderQueueClosed = errors.New("order queue closed")
	ErrOrderQueueFull   = errors.New("order queue's channel is full")
	ErrOrderQueueBusy   = errors.New("order queue's workers are all busy")
)

// Push push job to queue
func (q *OrderJobQueue) PushJob(k string, cb func()) error {
	n := atomic.LoadInt32(&q.closed)
	if n == orderqStatClosed || n == orderqStatGracefulClosed {
		return ErrOrderQueueClosed
	}

	j := &orderJob{
		key: k,
		fn:  cb,
	}
	var err error
	select {
	case q.ch <- j:
	default:
		err = ErrOrderQueueFull
	}
	return err
}

// Size return queue size
func (q *OrderJobQueue) Size() int {
	return q.size
}

// Close close queue
func (q *OrderJobQueue) Close() {
	b := atomic.CompareAndSwapInt32(&q.closed, orderqStatRunning, orderqStatClosed)
	if !b {
		return
	}
	// q.size + 1, extra 1 is for "loop" goroutine
	q.closeWg.Add(q.size + 1)
}

func (q *OrderJobQueue) Wait() {
	q.closeWg.Wait()
}

// GracefulClose graceful close queue.
// Queue exits when all jobs have done.
func (q *OrderJobQueue) GracefulClose() {
	b := atomic.CompareAndSwapInt32(&q.closed, orderqStatRunning, orderqStatGracefulClosed)
	if !b {
		return
	}
	// q.size + 1, extra 1 is for "loop" goroutine
	q.closeWg.Add(q.size + 1)
	q.closeWg.Wait()
}

func (q *OrderJobQueue) closeQueue() {
	q.closeWg.Done()
	for i := 0; i < q.size; i++ {
		q.closeCh <- true
	}
}

//loop要解决的问题：本质是一个线程池，完成回调函数而已
//1、当channel满时，任务被缓存在候补队列，等布尔值false时取出。为了防止channel满时Push操作阻塞。所以要Push进一个不会满的队列。用CAS来处理存取
//2、函数调用完就不用管了，map[string]bool仅用于判断是否正在工作，不用管
// perform only one job in one loop
func (q *OrderJobQueue) loop() {
	for {
		n := atomic.LoadInt32(&q.closed)
		if n == orderqStatClosed {
			q.closeQueue()
			return
		} else if n == orderqStatGracefulClosed {
			if q.bList.Len() == 0 && len(q.ch) == 0 {
				q.closeQueue()
				return
			}
		}

		//取出已经完成的调用
		select {
		case dn := <-q.doneCh:
			q.idleWorker = append(q.idleWorker, dn.ch) //加到空闲
			//清除锁
			delete(q.lockMap, dn.key)
		default:
			//没有已经完成的工作，走这里
		}

		// Handle the job of the backup list first, if the job's key isn't locked.
		// Otherwise, handle the job of queue channel.
		//从候补队列依次将任务取出，再次尝试将任务扔进工作队列
		e := q.bList.Front()
		if e != nil {
			j := e.Value.(*orderJob)
			if b := q.lockMap[j.key]; !b {
				// Get an idle worker to work
				ch, err := q.getCh()
				if err != nil {
					//fmt.Println(fmt.Sprintf("order queue went to the fatal logic: q.blist, key: %s", j.key))
				} else {
					q.bList.Remove(e)
					q.lockMap[j.key] = true
					ch <- j
				}
				continue
			}
		}

		//候补队列不为空，会一直检测直到清空候补队列
		if q.bList.Len() > 0 {
			select {
			case j := <-q.ch:
				q.doAJob(j)
			default:
			}
		} else {
			//候补队列为空，阻塞
			select {
			case j := <-q.ch:
				q.doAJob(j)
			}
		}
	}
}

func (q *OrderJobQueue) doAJob(j *orderJob) {
	// If the job's key locked, push job to backup list.
	//尝试获取锁（一个布尔值），
	if b := q.lockMap[j.key]; b {
		if q.bList.Len() >= q.size {
		} else {
			//未能获取锁，将任务推到候补队列(list)，返回
			q.bList.PushBack(j)
		}
		return
	}

	// Get an idle worker to work
	//
	ch, err := q.getCh()
	if err != nil {
		// Fatal: should not happend.
		// If all workers are busy, it would continue the loop in the previous code.
		//fmt.Println(fmt.Sprintf("order queue went to the fatal logic: q.ch, key: %s", j.key))
		return
	}

	q.lockMap[j.key] = true
	ch <- j
}

func (q *OrderJobQueue) getCh() (chan *orderJob, error) {
	ln := len(q.idleWorker)
	if ln == 0 {
		return nil, ErrOrderQueueBusy
	}
	ch := q.idleWorker[ln-1]
	q.idleWorker = q.idleWorker[:ln-1]
	if len(q.idleWorker) == 0 {
		//fmt.Printf("%v OrderJobQueue busy \n", time.Now())
	}
	return ch, nil
}
