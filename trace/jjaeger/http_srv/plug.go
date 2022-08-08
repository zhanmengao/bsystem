package main

import (
	"github.com/gin-gonic/gin"
	"github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/ext"
	"ttrace/jjaeger/ftracer"
)

// UseOpenTracing
//ref 就是多个 span 之间的关系。span 可以是跨进程的，也可以是一个进程内的不同函数中的。
//
//其中 span 的依赖关系表示示例：
//
//                    "references": [
//                        {
//                            "refType": "CHILD_OF",
//                            "traceID": "33ba35e7cc40172c",
//                            "spanID": "1c7826fa185d1107"
//                        }]
//spanID 为其依赖的父 span。
/*
一个进程中的 tracer 可以包装一些代码和操作，为多个 span 生成一些信息，或创建父子关系。

而 远程请求中传递的是 SpanContext，传递后，远程服务也创建新的 tracer，然后从 SpanContext 生成 span 依赖关系。

子 span 中，其 reference 列表中，会带有 父 span 的 span id。
*/
func UseOpenTracing() gin.HandlerFunc {
	handler := func(c *gin.Context) {
		// 使用 opentracing.GlobalTracer() 获取全局 Tracer
		tracer, spanContext, closer, _ := ftracer.CreateHttpTracer("userInfoWebService", c.Request.Header)
		defer closer.Close()
		// 生成依赖关系，并新建一个 span
		// 这里很重要，因为生成了  References []SpanReference 依赖关系
		startSpan := tracer.StartSpan(c.Request.URL.Path, ext.RPCServerOption(spanContext))
		defer startSpan.Finish()

		// 记录 tag
		// 记录请求 Url
		ext.HTTPUrl.Set(startSpan, c.Request.URL.Path)
		// Http Method
		ext.HTTPMethod.Set(startSpan, c.Request.Method)
		// 记录组件名称
		ext.Component.Set(startSpan, "Gin-Http")

		// 在 header 中加上当前进程的上下文信息
		c.Request = c.Request.WithContext(opentracing.ContextWithSpan(c.Request.Context(), startSpan))
		// 传递给下一个中间件
		c.Next()
		// 继续设置 tag
		ext.HTTPStatusCode.Set(startSpan, uint16(c.Writer.Status()))
	}

	return handler
}
