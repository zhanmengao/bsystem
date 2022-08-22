package main

import (
	"context"
	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/gtrace"
	"log"
	"trace/tracing"
)

var JaegerUdpEndpoint = "localhost:6831"

//创建tracer，生成root span
func main() {
	_, err := tracing.InitJaeger("test", JaegerUdpEndpoint)
	if err != nil {
		log.Fatal(err.Error())
	}

	ctx, span := gtrace.NewSpan(context.Background(), "main")
	defer span.End()

	user1 := GetUser(ctx, 1)
	g.Dump(user1)

	user100 := GetUser(ctx, 100)
	g.Dump(user100)
}

//上述代码创建了一个root span，并将该span通过context传递给GetUser方法，以便在GetUser方法中将追踪链继续延续下去。
