package main

import (
	"github.com/opentracing/opentracing-go"
	jaegercfg "github.com/uber/jaeger-client-go/config"
	"log"
)

func main() {
	// 从环境变量获取配置参数
	cfg, err := jaegercfg.FromEnv()
	if err != nil {
		log.Println(err)
	}
	cfg.Sampler = &jaegercfg.SamplerConfig{
		Type:  "const", // 使用const采样器
		Param: 1,       // 采样所有追踪
	}
	// 设置服务名
	cfg.ServiceName = "jaeger tracer demo"
	// 根据创建Tracer
	tracer, closer, err := cfg.NewTracer()
	if err != nil {
		log.Println(err)
	}
	defer closer.Close()
	//设置全局tracer
	opentracing.SetGlobalTracer(tracer)
	//创建一个span
	parentSpan := tracer.StartSpan("root")
	defer parentSpan.Finish()
	parentSpan.LogFields(
	//tracelog.String("hello", "world"),
	)
	parentSpan.LogKV("foo", "bar")
	// 创建一个childspan
	childspan := tracer.StartSpan("child span", opentracing.ChildOf(parentSpan.Context()))
	defer childspan.Finish()
}
