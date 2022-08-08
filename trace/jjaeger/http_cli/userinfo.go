package main

import (
	"github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/ext"
	"net/http"
)

// GetUserInfo 请求远程服务，获得用户信息
//使用Inject打包到Header中
func GetUserInfo(tracer opentracing.Tracer, parentSpan opentracing.Span) {
	// 继承上下文关系，创建子 span
	childSpan := tracer.StartSpan(
		"B",
		opentracing.ChildOf(parentSpan.Context()),
	)

	url := "http://127.0.0.1:8081/Get?username=lan"
	req, _ := http.NewRequest("GET", url, nil)
	// 设置 tag，这个 tag 我们后面讲
	ext.SpanKindRPCClient.Set(childSpan)
	ext.HTTPUrl.Set(childSpan, url)
	ext.HTTPMethod.Set(childSpan, "GET")
	_ = tracer.Inject(childSpan.Context(), opentracing.HTTPHeaders, opentracing.HTTPHeadersCarrier(req.Header))
	resp, _ := http.DefaultClient.Do(req)
	_ = resp // 丢掉
	defer childSpan.Finish()
}
