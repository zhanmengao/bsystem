package main

import (
	"github.com/opentracing/opentracing-go"
	"github.com/uber/jaeger-client-go"
	jaegercfg "github.com/uber/jaeger-client-go/config"
	jaegerlog "github.com/uber/jaeger-client-go/log"
)

//由于服务之间的调用是跨进程的，每个进程都有一些特点的标记，为了标识这些进程，我们需要在上下文间、span 携带一些信息。
func main() {

	cfg := jaegercfg.Configuration{
		ServiceName: "client test", // 对其发起请求的的调用链，叫什么服务
		Sampler: &jaegercfg.SamplerConfig{
			Type:  jaeger.SamplerTypeConst, //Sampler 是客户端采样率配置，可以通过 sampler.type 和 sampler.param 属性选择采样类型
			Param: 1,
		},
		Reporter: &jaegercfg.ReporterConfig{ //Reporter 可以配置如何上报
			LogSpans: true,
		},
	}

	jLogger := jaegerlog.StdLogger
	tracer, closer, err := cfg.NewTracer(
		jaegercfg.Logger(jLogger),
	)

	defer closer.Close()
	if err != nil {
	}

	// 创建第一个 span A
	parentSpan := tracer.StartSpan("A")
	defer parentSpan.Finish()

	B(tracer, parentSpan)
}

func B(tracer opentracing.Tracer, parentSpan opentracing.Span) {
	// 继承上下文关系，创建子 span
	childSpan := tracer.StartSpan(
		"B",
		opentracing.ChildOf(parentSpan.Context()),
	)
	defer childSpan.Finish()
}
