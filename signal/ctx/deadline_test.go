package ctx

import (
	"context"
	"fmt"
	"testing"
	"time"
)

/*
WithDeadline
返回父上下文的副本，并将deadline调整为不迟于d。如果父上下文的deadline已经早于d，则WithDeadline(parent, d)在语义上等同于父上下文。
当截止日过期时，当调用返回的cancel函数时，或者当父上下文的Done通道关闭时，返回上下文的Done通道将被关闭，以最先发生的情况为准。

取消此上下文将释放与其关联的资源，因此代码应该在此上下文中运行的操作完成后立即调用cancel。
*/

/*
上面的代码中，定义了一个50毫秒之后过期的deadline，然后我们调用context.WithDeadline(context.Background(), d)得到一个上下文（ctx）和一个取消函数（cancel），
然后使用一个select让主程序陷入等待：等待1秒后打印overslept退出或者等待ctx过期后退出。 因为ctx50秒后就过期，所以ctx.Done()会先接收到值，上面的代码会打印ctx.Err()取消原因。
*/
func TestWithDeadline(t *testing.T) {
	d := time.Now().Add(50 * time.Millisecond)
	ctx, cancel := context.WithDeadline(context.Background(), d)

	// 尽管ctx会过期，但在任何情况下调用它的cancel函数都是很好的实践。
	// 如果不这样做，可能会使上下文及其父类存活的时间超过必要的时间。
	defer cancel()

	select {
	case <-time.After(1 * time.Second):
		fmt.Println("overslept")
	case <-ctx.Done():
		fmt.Println(ctx.Err())
	}
}
