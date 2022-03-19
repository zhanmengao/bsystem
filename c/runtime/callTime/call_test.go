package callTime

import (
	"fmt"
	"math/rand"
	"testing"
	"time"
)

var callChan = make(chan CallInfo, 1000)

func TestCall1(t *testing.T) {
	//begin watch
	for i := 0; i < 3; i++ {
		go Watch(callChan)
	}

	//begin function
	for i := 0; i < 3; i++ {
		go func() {
			for {
				var info CallInfo
				info.Begin = time.Now().Unix()
				info.End = make(chan int64, 1)
				sl := rand.Intn(100000) + 500
				info.Info = fmt.Sprintf("function %d ", sl)
				callChan <- info
				handleFunc(sl)
				info.End <- time.Now().Unix()
			}
		}()
	}
	select {}
}

func handleFunc(tm int) {
	time.Sleep(time.Duration(tm) * time.Millisecond)
}

func TestCall2(t *testing.T) {
	mgr := NewCallManager()
	//begin function
	for i := 0; i < 3; i++ {
		go func() {
			for {
				sl := rand.Intn(5000)
				info := fmt.Sprintf("function %d ", sl)
				call := mgr.Push(info)
				handleFunc(sl)
				call.End = time.Now().Unix()
			}
		}()
	}
	mgr.Run()
}
