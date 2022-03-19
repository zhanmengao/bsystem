package freequeue

import (
	"fmt"
	"math/rand"
	"strconv"
	"sync/atomic"
	"testing"
	"time"
)

var count int64

const (
	Total = 10000000
	Mod   = 1000000
)

func TestNewOrderJobQueue(t *testing.T) {
	q := NewFreeQueue(1000)
	fmt.Printf("begin = %s \n", time.Now().String())
	for i := 0; i < Total; i++ {
		id := rand.Int63() + int64(i)
		q.Push(Int2Str(id), func() {
			newVal := atomic.AddInt64(&count, 1)
			if newVal%Mod == 0 {
				fmt.Printf("%d ok end = %s \n", count, time.Now().String())
			}
		})
	}
}
func BenchmarkNewOrderJobQueue(b *testing.B) {

}

func Int2Str(uid int64) string {
	s := strconv.FormatInt(uid, 10)
	return s
}
