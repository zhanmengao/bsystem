package freequeue

import (
	"fmt"
	"math/rand"
	"strconv"
	"sync"
	"sync/atomic"
	"testing"
	"time"
)

const (
	Total = 10000000
	Mod   = 1000000
)

func TestNewOrderJobQueue(t *testing.T) {
	var count int64
	q := NewFreeQueue(1000)
	fmt.Printf("begin = %s \n", time.Now().String())
	for i := 0; i < Total; i++ {
		id := rand.Int63() + int64(i)
		q.PushJob(Int2Str(id), func() {
			newVal := atomic.AddInt64(&count, 1)
			if newVal%Mod == 0 {
				fmt.Printf("%d ok end = %s \n", count, time.Now().String())
			}
		})
	}
}
func TestFrrQueue(t *testing.T) {
	var count int32 = 0
	q := NewFreeQueue(1000)
	addWg := sync.WaitGroup{}
	q.Run()
	addWg.Add(10000 * 1000)
	begin := time.Now()
	for i := 0; i < 10000; i++ {
		go func() {
			for j := 0; j < 1000; j++ {
				q.PushJob(Int2Str(int64(j)), func() {
					atomic.AddInt32(&count, 1)
				})
				addWg.Done()
			}
		}()
	}
	addWg.Wait()
	q.Close()
	q.Wait()
	use := time.Now().Sub(begin)
	fmt.Println(count, use.String())
	if count != int32(10000*1000) {
		t.Fail()
	}
}

func Int2Str(uid int64) string {
	s := strconv.FormatInt(uid, 10)
	return s
}
