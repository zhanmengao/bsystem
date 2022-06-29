package main

import (
	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/ghttp"
	"github.com/gogf/gf/v2/net/gtrace"
	"github.com/gogf/gf/v2/os/gctx"

	"github.com/gogf/gf-tracing/tracing"
)

const (
	ServiceName       = "tracing-http-server"
	JaegerUdpEndpoint = "localhost:6831"
)

func main() {
	ctx := gctx.New()
	tp, err := tracing.InitJaeger(ServiceName, JaegerUdpEndpoint)
	if err != nil {
		g.Log().Fatal(ctx, err)
	}

	defer tp.Shutdown(ctx)

	s := g.Server()
	s.Group("/", func(group *ghttp.RouterGroup) {
		group.GET("/hello", HelloHandler)
	})
	s.SetPort(8199)
	s.Run()
}

// HelloHandler hello handler
func HelloHandler(r *ghttp.Request) {
	ctx, span := gtrace.NewSpan(r.Context(), "HelloHandler")
	defer span.End()

	value := gtrace.GetBaggageVar(ctx, "name").String()

	r.Response.Write("hello:", value)
}
