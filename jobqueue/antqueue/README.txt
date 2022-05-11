初始化 goroutine 池；
提交任务给 goroutine 池，检查是否有空闲的 goroutine：

有，获取空闲 goroutine
无，检查池中的 goroutine 数量是否已到池容量上限：

已到上限，检查 goroutine 池是否是非阻塞的：

非阻塞，直接返回nil表示执行失败
阻塞，等待 goroutine 空闲
未到上限，创建一个新的 goroutine 处理任务
任务处理完成，将 goroutine 交还给池，以待处理下一个任务

ants提供了一些选项可以定制 goroutine 池的行为。选项使用Options结构定义：

// src/github.com/panjf2000/ants/options.go
type Options struct {
    ExpiryDuration time.Duration
    PreAlloc bool
    MaxBlockingTasks int
    Nonblocking bool
    PanicHandler func(interface{})
    Logger Logger
}
各个选项含义如下：

ExpiryDuration：过期时间。表示 goroutine 空闲多长时间之后会被ants池回收
PreAlloc：预分配。调用NewPool()/NewPoolWithFunc()之后预分配worker（管理一个工作 goroutine 的结构体）切片。而且使用预分配与否会直接影响池中管理worker的结构。见下面源码
MaxBlockingTasks：最大阻塞任务数量。即池中 goroutine 数量已到池容量，且所有 goroutine 都处理繁忙状态，这时到来的任务会在阻塞列表等待。这个选项设置的是列表的最大长度。阻塞的任务数量达到这个值后，后续任务提交直接返回失败
Nonblocking：池是否阻塞，默认阻塞。提交任务时，如果ants池中 goroutine 已到上限且全部繁忙，阻塞的池会将任务添加的阻塞列表等待（当然受限于阻塞列表长度，见上一个选项）。非阻塞的池直接返回失败
PanicHandler：panic 处理。遇到 panic 会调用这里设置的处理函数
Logger：指定日志记录器

使用预分配时，创建loopQueueType类型的结构，反之创建stackType类型。这是ants定义的两种管理worker的数据结构。

ants定义了一些With*函数来设置这些选项：


这里使用了 Go 语言中非常常见的一种模式，我称之为选项模式，非常方便地构造有大量参数，且大部分有默认值或一般不需要显式设置的对象。

我们来验证几个选项。

最大等待队列长度
ants池设置容量之后，如果所有的 goroutine 都在处理任务。这时提交的任务默认会进入等待队列，WithMaxBlockingTasks(maxBlockingTasks int)可以设置等待队列的最大长度。超过这个长度，提交任务直接返回错误：


上面代码中，我们设置 goroutine 池的容量为 4，最大阻塞队列长度为 2。然后一个 for 提交 8 个任务，期望结果是：4 个任务在执行，2 个任务在等待，2 个任务提交失败。运行结果：


代码中有 4 点需要注意：

提交任务必须并行进行。如果是串行提交，第 5 个任务提交时由于池中没有空闲的 goroutine 处理该任务，Submit()方法会被阻塞，后续任务就都不能提交了。也就达不到验证的目的了
由于任务可能提交失败，失败的任务不会实际执行，所以实际上wg.Done()次数会小于 8。因而在err != nil分支中我们需要调用一次wg.Done()。否则wg.Wait()会永远阻塞
为了避免任务执行过快，空出了 goroutine，观察不到现象，每个任务中我使用time.Sleep(1 * time.Second)休眠 1s
由于 goroutine 之间的执行顺序未显式同步，故每次执行的顺序不确定
由于简单起见，前面的例子中Submit()方法的返回值都被我们忽略了。实际开发中一定不要忽略。

非阻塞
ants池默认是阻塞的，我们可以使用WithNonblocking(nonblocking bool)设置其为非阻塞。非阻塞的ants池中，在所有 goroutine 都在处理任务时，提交新任务会直接返回错误：


一个鲁棒性强的库一定不会忽视错误的处理，特别是宕机相关的错误。在 Go 语言中就是 panic，也被称为运行时恐慌，在程序运行的过程中产生的严重性错误，例如索引越界，空指针解引用等，都会触发 panic。如果不处理 panic，程序会直接意外退出，可能造成数据丢失的严重后果。

ants中如果 goroutine 在执行任务时发生panic，会终止当前任务的执行，将发生错误的堆栈输出到os.Stderr。注意，该 goroutine 还是会被放回池中，下次可以取出执行新的任务。


我们让偶数个任务触发panic。提交两个任务，第二个任务一定会触发panic。触发panic之后，我们还可以继续提交任务 3、5。注意这里没有 4，提交任务 4 还是会触发panic。

上面的程序需要注意 2 点：

任务函数中wg.Done()是在panic方法之后，如果触发了panic，函数中的其他正常逻辑就不会再继续执行了。所以我们虽然wg.Add(3)，但是一共提交了 4 个任务，其中一个任务触发了panic，wg.Done()没有正确执行。实际开发中，我们一般使用defer语句来确保wg.Done()一定会执行
在 for 循环之后，我添加了一行代码time.Sleep(1 * time.Second)。如果没有这一行，后续的两条Submit()方法可以直接执行，可能会导致任务很快就完成了，wg.Wait()直接返回了，这时panic的堆栈还没有输出。你可以尝试注释掉这行代码运行看看结果
除了ants提供的默认 panic 处理器，我们还可以使用WithPanicHandler(paincHandler func(interface{}))指定我们自己编写的 panic 处理器。处理器的参数就是传给panic的值：


看到输出了传给panic函数的字符串（第二行输出）。

默认池
为了方便使用，很多 Go 库都喜欢提供其核心功能类型的一个默认实现。可以直接通过库提供的接口调用。例如net/http，例如ants。ants库中定义了一个默认的池，默认容量为MaxInt32。goroutine 池的各个方法都可以直接通过ants包直接访问：

// src/github.com/panjf2000/ants/ants.go
defaultAntsPool, _ = NewPool(DefaultAntsPoolSize)

func Submit(task func()) error {
return defaultAntsPool.Submit(task)
}

func Running() int {
return defaultAntsPool.Running()
}

func Cap() int {
return defaultAntsPool.Cap()
}

func Free() int {
return defaultAntsPool.Free()
}

func Release() {
defaultAntsPool.Release()
}

func Reboot() {
defaultAntsPool.Reboot()
}
直接使用：

func main() {
defer ants.Release()

var wg sync.WaitGroup
wg.Add(2)
for i := 1; i <= 2; i++ {
ants.Submit(wrapper(i, &wg))
}
wg.Wait()
}
默认池也需要Release()。