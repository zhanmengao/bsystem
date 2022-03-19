package goMutex

import (
	"fmt"
	"sync"
	"testing"
	"time"
)
/*
假设有10个并发的协程打印了同一个数字100，为了避免重复打印，实现了printOnce(num int) 函数，使用集合 set 记录已打印过的数字，如果数字已打印过，则不再打印。
 */
var m sync.Mutex
var set = make(map[int]bool,0)

func printOnce(num int){
	m.Lock()
	defer m.Unlock()
	if _,exist := set[num];!exist{
		fmt.Println(num)
	}
	set[num] = true

}

func TestOne(t *testing.T) {
	for i:=0;i<10;i++{
		go printOnce(100)
	}
	time.Sleep(time.Second)
}