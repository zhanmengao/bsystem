package antsqueue

import (
	"fmt"
	"sync/atomic"
	"testing"
	"time"
)

func TestAntsQueue(t *testing.T) {
	var count int32 = 0
	q := NewAntsQueue(1000)
	go q.Run()
	begin := time.Now()
	for i := 0; i < 10000; i++ {
		go func() {
			for j := 0; j < 1000; j++ {
				q.PushJob(func() {
					atomic.AddInt32(&count, 1)
				})
			}
		}()
	}
	q.Close()
	q.Wait()
	use := time.Now().Sub(begin)
	fmt.Println(count, use.String())
	if count != int32(10000*1000) {
		t.Fail()
	}
}
