package main

import (
	"fmt"
	"github.com/panjf2000/ants/v2"
	"os"
	"sync"
	"testing"
	"time"
)

//ants中如果 goroutine 在执行任务时发生panic，会终止当前任务的执行，将发生错误的堆栈输出到os.Stderr。
//该 goroutine 还是会被放回池中，下次可以取出执行新的任务。
func TestPanic(t *testing.T) {
	p, _ := ants.NewPool(2)
	defer p.Release()

	var wg sync.WaitGroup
	wg.Add(3)
	for i := 1; i <= 2; i++ {
		p.Submit(wrapperPanic(i, &wg))
	}

	time.Sleep(1 * time.Second)
	p.Submit(wrapper(3, &wg))
	p.Submit(wrapper(5, &wg))
	wg.Wait()
}

//除了ants提供的默认 panic 处理器，我们还可以使用WithPanicHandler(paincHandler func(interface{}))指定我们自己编写的 panic 处理器。处理器的参数就是传给panic的值
func panicHandler(err interface{}) {
	fmt.Fprintln(os.Stderr, err)
}
func TestPanicHandler(t *testing.T) {
	p, _ := ants.NewPool(2, ants.WithPanicHandler(panicHandler))
	defer p.Release()
	var wg sync.WaitGroup
	wg.Add(3)
	for i := 1; i <= 2; i++ {
		p.Submit(wrapperPanic(i, &wg))
	}

	time.Sleep(1 * time.Second)
	p.Submit(wrapper(3, &wg))
	p.Submit(wrapper(5, &wg))
	wg.Wait()
}
