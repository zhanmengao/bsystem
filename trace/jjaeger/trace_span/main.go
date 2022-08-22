package main

import "github.com/opentracing/opentracing-go"

/*
从形式上看，Dapper 跟踪模型使用的是树形结构，Span 以及 Annotation。

在前面的图片中，我们可以看到，整个请求网络是一个树形结构，用户请求是树的根节点。在 Dapper 的跟踪树结构中，树节点是整个架构的基本单元。

span 称为跨度，一个节点在收到请求以及完成请求的过程是一个 span，span 记录了在这个过程中产生的各种信息。每个节点处理每个请求时都会生成一个独一无二的的 span id，
当 A -> C -> D 时，多个连续的 span 会产生父子关系，那么一个 span 除了保存自己的 span id，也需要关联父、子 span id。生成 span id 必须是高性能的，并且能够明确表示时间顺序。

Annotation 译为注释，在一个 span 中，可以为 span 添加更多的跟踪细节，这些额外的信息可以帮助我们监控系统的行为或者帮助调试问题。Annotation 可以添加任意内容。

*/

/*
Span
Span 由以下信息组成：

An operation name：操作名称，必有；
A start timestamp：开始时间戳，必有；
A finish timestamp：结束时间戳，必有；
Span Tags.：Key-Value 形式表示请求的标签，可选；
Span Logs：Key-Value 形式表示，记录简单的、结构化的日志，必须是字符串类型，可选；
SpanContext ：跨度上下文，在不同的 span 中传递，建立关系；
References t：引用的其它 Span；
span 之间如果是父子关系，则可以使用 SpanContext 绑定这种关系。父子关系有 ChildOf、FollowsFrom 两种表示，ChildOf 表示 父 Span 在一定程度上依赖子 Span，而 FollowsFrom 表示父 Span 完全不依赖其子Span 的结果。
*/

/*
Trace
一个简化的 Trace 如下：

注：不同编程语言的字段名称有所差异，gRPC 和 Restful API 的格式也有所差异。

            "traceID": "790e003e22209ca4",
            "spans":[...],
            "processes":{...}
前面说到，在 OpenTracing 中，Trace 是一个有向非循环图，那么 Trace 必定有且只有一个起点。

这个起点会创建一个 Trace 对象，这个对象一开始初始化了 trace id 和 process，trace id 是一个 32 个长度的字符串组成，它是一个时间戳，而 process 是起点进程所在主机的信息。
*/
func main() {
	tracer := opentracing.GlobalTracer() // jaeger 包中有个函数可以创建空的 trace。生产中不要使用
	// 创建第一个 span A
	parentSpan := tracer.StartSpan("A")
	defer parentSpan.Finish() // 可手动调用 Finish()

}

//每个 span 表示调用链中的一个结点，每个结点都需要明确父 span。
//
//现在，我们知道了，如何生成 trace{span1,span2}，且 span1 -> span2 即 span1 调用 span2，或 span1 依赖于 span2。

func B(tracer opentracing.Tracer, parentSpan opentracing.Span) {
	// 继承上下文关系，创建子 span
	childSpan := tracer.StartSpan(
		"B",
		opentracing.ChildOf(parentSpan.Context()),
	)
	defer childSpan.Finish() // 可手动调用 Finish()
}
