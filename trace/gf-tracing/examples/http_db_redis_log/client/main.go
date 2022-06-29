package main

import (
	"context"

	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/gtrace"

	"github.com/gogf/gf-tracing/tracing"
)

const (
	ServiceName       = "tracing-http-client"
	JaegerUdpEndpoint = "localhost:6831"
)

func main() {
	tp, err := tracing.InitJaeger(ServiceName, JaegerUdpEndpoint)
	if err != nil {
		g.Log().Fatal(context.Background(), err)
	}
	defer tp.Shutdown(context.Background())

	StartRequests()
}

func StartRequests() {
	ctx, span := gtrace.NewSpan(context.Background(), "StartRequests")
	defer span.End()

	client := g.Client()
	// Add user info.
	idStr := client.PostContent(ctx,
		"http://127.0.0.1:8199/user/insert",
		g.Map{
			"name": "john",
		},
	)
	if idStr == "" {
		g.Log().Print(ctx, "retrieve empty id string")
		return
	}
	g.Log().Print(ctx, "insert:", idStr)

	// Query user info.
	userJson := client.GetContent(ctx,
		"http://127.0.0.1:8199/user/query",
		g.Map{
			"id": idStr,
		},
	)
	g.Log().Print(ctx, "query:", idStr, userJson)

	// Delete user info.
	deleteResult := client.PostContent(ctx,
		"http://127.0.0.1:8199/user/delete",
		g.Map{
			"id": idStr,
		},
	)
	g.Log().Print(ctx, "delete:", idStr, deleteResult)
}
