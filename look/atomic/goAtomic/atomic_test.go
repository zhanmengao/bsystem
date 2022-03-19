package goAtomic

import (
	"fmt"
	"sync"
	"sync/atomic"
	"testing"
	"time"
)
/*
v.Store(c) - 写操作，将原始的变量c存放到一个atomic.Value类型的v里。
c = v.Load() - 读操作，从线程安全的v中读取上一步存放的内容。
 */


//应用程序定期的从外界获取最新的配置信息，然后更改自己内存中维护的配置变量。工作线程根据最新的配置来处理请求。


var lock sync.Mutex
var ch = make(chan struct{},2)
func threadLock(num *int64) {
	fmt.Printf("begin time %v \n", time.Now())
	for i := 0; i < 100000000; i++ {
		lock.Lock()
		*num++
		lock.Unlock()
	}
	fmt.Printf("end time %v \n", time.Now())
	ch <- struct{}{}
}

func threadAtomic(num *int64) {
	fmt.Printf("begin time %v \n", time.Now())
	for i := 0; i < 100000000; i++ {
		atomic.AddInt64(num, 1)
	}
	fmt.Printf("end time %v \n", time.Now())
	ch <- struct{}{}
}

func TestAtomicCpp(t *testing.T) {
	var num int64 =0
	go threadLock(&num)
	go threadLock(&num)
	for i:=0;i<2;i++{
		<-ch
	}
	fmt.Println("last num = ",num)

	num = 0
	go threadAtomic(&num)
	go threadAtomic(&num)
	for i:=0;i<2;i++{
		<-ch
	}
	fmt.Println("last num = ",num)
}
