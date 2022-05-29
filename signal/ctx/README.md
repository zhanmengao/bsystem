Go1.7加入了一个新的标准库context，它定义了Context类型，专门用来简化 对于处理单个请求的多个 goroutine 之间与请求域的数据、取消信号、截止时间等相关操作，这些操作可能涉及多个 API 调用。

对服务器传入的请求应该创建上下文，而对服务器的传出调用应该接受上下文。它们之间的函数调用链必须传递上下文，或者可以使用WithCancel、WithDeadline、WithTimeout或WithValue创建的派生上下文。当一个上下文被取消时，它派生的所有上下文也被取消。



    Deadline方法需要返回当前Context被取消的时间，也就是完成工作的截止时间（deadline）；
    Done方法需要返回一个Channel，这个Channel会在当前工作完成或者上下文被取消之后关闭，多次调用Done方法会返回同一个Channel；
    Err方法会返回当前Context结束的原因，它只会在Done返回的Channel被关闭时才会返回非空的值；
        如果当前Context被取消就会返回Canceled错误；
        如果当前Context超时就会返回DeadlineExceeded错误；
    Value方法会从Context中返回键对应的值，对于同一个上下文来说，多次调用Value 并传入相同的Key会返回相同的结果，该方法仅用于传递跨API和进程间跟请求域的数据；


Go内置两个函数：Background()和TODO()，这两个函数分别返回一个实现了Context接口的background和todo。我们代码中最开始都是以这两个内置的上下文对象作为最顶层的partent context，衍生出更多的子上下文对象。

Background()主要用于main函数、初始化以及测试代码中，作为Context这个树结构的最顶层的Context，也就是根Context。

TODO()，它目前还不知道具体的使用场景，如果我们不知道该使用什么Context的时候，可以使用这个。

background和todo本质上都是emptyCtx结构体类型，是一个不可取消，没有设置截止时间，没有携带任何值的Context。


使用Context的注意事项

    推荐以参数的方式显示传递Context
    以Context作为参数的函数方法，应该把Context作为第一个参数。
    给一个函数方法传递Context的时候，不要传递nil，如果不知道传递什么，就使用context.TODO()
    Context的Value相关方法应该传递请求域的必要数据，不应该用于传递可选参数
    Context是线程安全的，可以放心的在多个goroutine中传递
