package jobqueue

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"sync"
	"sync/atomic"
	"testing"
	"time"
)

func TestQueue(t *testing.T) {
	jobQueue, err := NewOrderJobQueue(10000)
	if err != nil {
		return
	}
	jobQueue.Push("1", func() {
		fmt.Println("hello world")
	})
	time.Sleep(time.Duration(3) * time.Second)
}

const (
	gCount   = 10
	countLen = 1000
)

func TestQueue2(t *testing.T) {
	jobQueue, err := NewOrderJobQueue(1000)
	if err != nil {
		return
	}

	var wg sync.WaitGroup
	var count int64 = 0
	for i := 0; i < gCount; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for j := 0; j < countLen; j++ {
				uid := Int2Str(rand.Int63n(1000000))
				jobQueue.Push(uid, func() {
					atomic.AddInt64(&count, 1)
					if atomic.LoadInt64(&count) >= int64(gCount*countLen-1) {
						fmt.Printf("call end %d \n", count)
						os.Exit(1)
					}
				})
			}
		}()
	}
	wg.Wait()
	time.Sleep(time.Duration(10) * time.Second)
	fmt.Printf("main end %d \n", count)
}

func Int2Str(uid int64) string {
	s := strconv.FormatInt(uid, 10)
	return s
}

var count int64

const (
	Total = 10000000
	Mod   = 1000000
)

func TestNewOrderJobQueue(t *testing.T) {
	q, err := NewOrderJobQueue(1000)
	if err != nil {
		t.Fatal(err)
	}
	fmt.Printf("begin = %s \n", time.Now().String())
	for i := 0; i < Total; i++ {
		id := rand.Int63() + int64(i)
		err = q.Push(Int2Str(id), func() {
			newVal :=atomic.AddInt64(&count, 1)
			if newVal%Mod == 0 {
				fmt.Printf("%d ok end = %s \n", count, time.Now().String())
			}
		})
	}
}
func BenchmarkNewOrderJobQueue(b *testing.B) {
	q, err := NewOrderJobQueue(1000)
	if err != nil {
		b.Fatal(err)
	}
	tm := time.Now().Unix()
	fmt.Printf("begin = %s ", time.Unix(tm, 0).String())
	for i := 0; i < Total; i++ {
		id := rand.Int63n(tm) + int64(i)
		err = q.Push(Int2Str(id), func() {
			newVal :=atomic.AddInt64(&count, 1)
			if newVal%Mod == 0 {
				fmt.Printf("%d ok end = %s \n", count, time.Now().String())
			}
		})
	}
}
