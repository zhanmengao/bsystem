package tm

import (
	"container/list"
	"sync"
	"time"

	"forevernine.com/midplat/base_server/freequeue"
)

var (
	lock     sync.Mutex
	onceList *list.List
	wake     chan *timerJob
	timer    *time.Timer
	jobQueue *freequeue.FreeQueue
	index    int
)
