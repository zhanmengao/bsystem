package tm

import (
	"container/list"
	"github.com/panjf2000/ants/v2"
	"sync"
	"time"
)

var (
	lock     sync.Mutex
	onceList *list.List
	wake     chan *timerJob
	timer    *time.Timer
	jobQueue *ants.Pool
	index    int
)
