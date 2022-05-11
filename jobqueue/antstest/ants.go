package main

import (
	"github.com/panjf2000/ants/v2"
	"sync"
)

//为了方便使用，很多 Go 库都喜欢提供其核心功能类型的一个默认实现。可以直接通过库提供的接口调用。例如net/http
//ants库中定义了一个默认的池，默认容量为MaxInt32。goroutine 池的各个方法都可以直接通过ants包直接访问：
//调默认池.默认池也需要Release()。
func main() {
	defer ants.Release()

	var wg sync.WaitGroup
	wg.Add(2)
	for i := 1; i <= 2; i++ {
		ants.Submit(wrapper(i, &wg))
	}
	wg.Wait()
}
