package callTime

import (
	"container/list"
	"fmt"
	"sync"
	"time"
)

type CallInfo struct {
	Begin int64
	Info  string
	End   chan int64
}

func Watch(callChan chan CallInfo) {
	for {
		info := <-callChan
		for {
			bEnd := false
			select {
			case <-time.After(time.Duration(500) * time.Millisecond):
				{
					fmt.Printf("%s process %d \n", info.Info, time.Since(time.Unix(info.Begin, 0))/time.Second)
				}
			case end := <-info.End:
				{
					fmt.Printf("%s process use %d \n", info.Info, end-info.Begin)
					bEnd = true
				}
			}
			if bEnd {
				break
			}
		}
	}
}

type CallInfoItem struct {
	Begin int64
	Info  string
	End   int64
}
type CallManager struct {
	running *list.List
	lock sync.Mutex
}

func NewCallManager() *CallManager {
	return &CallManager{
		running: list.New(),
	}
}

func (mgr *CallManager) Push(info string) *CallInfoItem {
	call := &CallInfoItem{
		Info:  info,
		Begin: time.Now().Unix(),
	}
	mgr.lock.Lock()
	defer mgr.lock.Unlock()
	mgr.running.PushBack(call)
	return call
}

func (mgr *CallManager) Run() {
	for{
		now := time.Now().Unix()
		mgr.lock.Lock()
		for item := mgr.running.Front(); item != nil; {
			call := item.Value.(*CallInfoItem)
			if call.End != 0 {
				delItem := item
				item = item.Next()
				mgr.running.Remove(delItem)
			} else {
				if (now - call.Begin) >= 1 {
					fmt.Printf("%s running . use time = %d \n", call.Info, now-call.Begin)
				}
				item = item.Next()
			}
		}
		mgr.lock.Unlock()
		time.Sleep(time.Second * time.Duration(1))
	}
}
