package main

import (
	"bufio"
	"github.com/opentracing/opentracing-go"
	"github.com/uber/jaeger-client-go"
	jaegercfg "github.com/uber/jaeger-client-go/config"
	jaegerlog "github.com/uber/jaeger-client-go/log"
	"os"
	"ttrace/jjaeger/def"
)

/*
sampler 可以使用 jaegercfg.SamplerConfig，通过 type、param 两个字段来配置采样器。

为什么要配置采样器？因为服务中的请求千千万万，如果每个请求都要记录追踪信息并发送到 Jaeger 后端，那么面对高并发时，记录链路追踪以及推送追踪信息消耗的性能就不可忽视，会对系统带来较大的影响。当我们配置 sampler 后，jaeger 会根据当前配置的采样策略做出采样行为
"const"	0或1	采样器始终对所有 tracer 做出相同的决定；要么全部采样，要么全部不采样
"probabilistic"	0.0~1.0	采样器做出随机采样决策，Param 为采样概率
"ratelimiting"	N	采样器一定的恒定速率对tracer进行采样，Param=2.0，则限制每秒采集2条
"remote"	无	采样器请咨询Jaeger代理以获取在当前服务中使用的适当采样策略。
sampler.Type="remote"/sampler.Type=jaeger.SamplerTypeRemote 是采样器的默认值，当我们不做配置时，会从 Jaeger 后端中央配置甚至动态地控制服务中的采样策略。
*/

/*
QUEUESIZE，设置队列大小，存储采样的 span 信息，队列满了后一次性发送到 jaeger 后端；defaultQueueSize 默认为 100；

BufferFlushInterval 强制清空、推送队列时间，对于流量不高的程序，队列可能长时间不能满，那么设置这个时间，超时可以自动推送一次。对于高并发的情况，一般队列很快就会满的，满了后也会自动推送。默认为1秒。

LogSpans 是否把 Log 也推送，span 中可以携带一些日志信息。

LocalAgentHostPort 要推送到的 Jaeger agent，默认端口 6831，是 Jaeger 接收压缩格式的 thrift 协议的数据端口。

CollectorEndpoint 要推送到的 Jaeger Collector，用 Collector 就不用 agent 了。
*/
func main() {

	var cfg = jaegercfg.Configuration{
		ServiceName: "client test", // 对其发起请求的的调用链，叫什么服务
		Sampler: &jaegercfg.SamplerConfig{
			Type:  jaeger.SamplerTypeConst,
			Param: 1,
		},
		Reporter: &jaegercfg.ReporterConfig{
			LogSpans:          true,
			CollectorEndpoint: def.JaegerAddr,
		},
	}

	jLogger := jaegerlog.StdLogger
	tracer, closer, _ := cfg.NewTracer(
		jaegercfg.Logger(jLogger),
	)

	// 创建第一个 span A
	parentSpan := tracer.StartSpan("A")
	// 调用其它服务
	B(tracer, parentSpan)
	// 结束 A
	parentSpan.Finish()
	// 结束当前 tracer
	closer.Close()

	reader := bufio.NewReader(os.Stdin)
	_, _ = reader.ReadByte()
}
func B(tracer opentracing.Tracer, parentSpan opentracing.Span) {
	// 继承上下文关系，创建子 span
	childSpan := tracer.StartSpan(
		"B",
		opentracing.ChildOf(parentSpan.Context()),
	)
	defer childSpan.Finish()
}
