package main

import (
	"context"

	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/gtrace"
	"github.com/gogf/gf/v2/os/gctx"

	"github.com/gogf/gf-tracing/tracing"
)

const (
	// ServiceName .
	ServiceName = "tracing-http-client"
	// JaegerUDPEndpoint .
	JaegerUDPEndpoint = "localhost:6831"
)

func main() {
	ctx := gctx.New()
	tp, err := tracing.InitJaeger(ServiceName, JaegerUDPEndpoint)
	if err != nil {
		g.Log().Fatal(ctx, err)
	}
	defer tp.Shutdown(ctx)

	StartRequests()
}

// StartRequests start request
func StartRequests() {
	ctx, span := gtrace.NewSpan(context.Background(), "StartRequests")
	defer span.End()

	ctx = gtrace.SetBaggageValue(ctx, "name", "john")

	content := g.Client().GetContent(ctx, "http://127.0.0.1:8199/hello")

	g.Log().Print(ctx, content)
}
